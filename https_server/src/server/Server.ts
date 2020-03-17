import http     = require("http");
import express  = require("express");
import path     = require("path");


/**
 * 
 */
export class Server {

    protected readonly app: express.Application;
    protected readonly server: http.Server;

    /**
     * How to generate security keys to host the server from your machine:
     * (we don't want to push these to the repo)
     * Save these files to the `.security_keys` folder.
     * https://nodejs.org/en/knowledge/HTTP/servers/how-to-create-a-HTTPS-server/
     * 
     * https://en.wikipedia.org/wiki/0.0.0.0#As_a_host_address
     */
    public constructor() {
        this.app = express();

        // Create an HTTPS server on localhost port 8000
        this.server = http.createServer({
            // key:  fs.readFileSync(path.resolve(__dirname, "../../.security/key.pem")),
            // cert: fs.readFileSync(path.resolve(__dirname, "../../.security/cert.pem")),
        }, this.app).listen(Server.PORT);

        this.app.use(express.static(path.resolve(__dirname, "../..")));
        this.app.get("/", (req, res) => {
            res.sendFile(path.resolve(__dirname, "../../index.html"));
        });

        // console.log(Object.values(os.networkInterfaces()).flat()
        //     .filter((desc) => /^(10|172|192)\.*/.test(desc.address))
        //     .map((info) => info.address + ":" + Server.PORT));
        // this.app.listen(Server.PORT, "localhost"); // For private network, use "0.0.0.0"
        console.log(this.server.address());
    }

}


export namespace Server {
    export const PORT = 80;
}