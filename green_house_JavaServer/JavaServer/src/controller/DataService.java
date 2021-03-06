package controller;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.time.Instant;

/*
 * Data Service as a vertx event-loop 
 */
public class DataService extends AbstractVerticle{

	private final int port;
	private final ObservableHumidityAgent hAgent;
	public DataService(final int port, final ObservableHumidityAgent hAgent) {		
		this.port = port;
		this.hAgent = hAgent;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data").handler(this::handleGetData);		
		vertx
			.createHttpServer()
			.requestHandler(router::accept)
			.listen(port);

		log("Service ready.");
	}
	
	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		JsonObject res = routingContext.getBodyAsJson();
		if (res == null) {
			sendError(400, response);
		} else {
			if(res.getInteger("Humidity") != null) {
				try {
					log(res.getInteger("Humidity").toString());
					save(res.getInteger("Humidity").toString());
					hAgent.sendUmMsg(res.getInteger("Humidity"));
					hAgent.checkMin(res.getInteger("Humidity"));
				} catch (IOException e) {
					e.printStackTrace();
				}
							
				response.setStatusCode(200).end();
			} else {
				response.setStatusCode(400).end();
			}
		}
	}
	
	private void handleGetData(RoutingContext routingContext) {
		routingContext.response()
			.setStatusCode(400).end();
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[DATA SERVICE] "+msg);
	}

	private void save(String msg) throws IOException {
		final BufferedWriter bw = new BufferedWriter(new FileWriter(new File("/opt/lampp/htdocs/umid.txt"), true));
        bw.append(Instant.now() + " " + msg + "\n");
        bw.close();
	}
}