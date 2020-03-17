
export class Client {

    protected readonly scoreboard: HTMLTableElement;

    public constructor() {
        (document.getElementById("ping-button") as HTMLButtonElement).onclick = this.sendPingToDe1;;
        (document.getElementById("register-user-button") as HTMLButtonElement).onclick = this.registerUser;;
        {
            this.scoreboard = document.createElement("table");
            this.scoreboard.createTBody();
        } {
            const input = document.getElementById("de1-ip-address-input") as HTMLInputElement;
            input.value = input.placeholder;
        }
        document.getElementById("scoreboard")?.appendChild(this.scoreboard);
    }

    /**
     * https://developer.mozilla.org/en-US/docs/Web/API/Geolocation_API/Using_the_Geolocation_API
     * https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
     */
    protected sendPingToDe1(): any {
        const ipAddress = (document.getElementById("de1-ip-address-input") as HTMLInputElement).value;
        navigator.geolocation.getCurrentPosition(
            (position: Position): void => {
                document.getElementById("user-latitude")!.innerText  = position.coords.latitude.toPrecision(7);
                document.getElementById("user-longitude")!.innerText = position.coords.longitude.toPrecision(7);
                document.getElementById("ping-response")!.innerText  = "waiting...";
                //console.log(ipAddress);
                sendEmptyFetch("http://" + ipAddress).then();
                fetchProximity("http://" + ipAddress, position.coords)
                .then((data) => data.text())
                .then((text) => {
                    text = text.trim();
                    document.getElementById("ping-response")!.innerText = text;
                });
            },
            Client.GetPosition.ON_ERROR,
            Client.GetPosition.OPTIONS,
        );
    }

    protected registerUser(): any {
        const ipAddress = (document.getElementById("de1-ip-address-input") as HTMLInputElement).value;
        const username = (document.getElementById("username-input") as HTMLInputElement).value.trim();
        if (!(/^[a-zA-Z0-9_\- ]+$/.test(username) && username.length <= Client.USERNAME_MAX_LENGTH)) {
            // Invalid username.
            window.alert("Please choose a 1-11 character-long username."
                + "\nIt can contain letters, numbers, underscores, dashes, and spaces.");
            return;
        }
        sendEmptyFetch("http://" + ipAddress).then();
        fetchUserPinNumber("http://" + ipAddress, username)
        .then((data) => data.text())
        .then((text) => {
            text = text.trim();
            while (text.length < 4) {
                text = "0" + text;
            }
            (document.getElementById("pin-display") as HTMLHeadingElement).innerHTML = `Your pin is: ${text}`;
        });
    }

}

async function sendEmptyFetch(url: string): Promise<Response> {
    return fetch(url, createFetchRequestInit(""));
}

async function fetchProximity(url: string, coords: Coordinates): Promise<Response> {
    const body = `${Client.DE1_COMMS_CMDS.PING}%${coords.latitude}%${coords.longitude}%` + "$";
    console.log(body, body.length);
    return fetch(url, createFetchRequestInit(body));
}

async function fetchUserPinNumber(url: string, username: string): Promise<Response> {
    const body = `${Client.DE1_COMMS_CMDS.REGISTER_USER}%${username}` + "$";
    console.log(body, body.length);
    return fetch(url, createFetchRequestInit(body));
}

const createFetchRequestInit = (body: string): RequestInit => {
    return {
        method: 'POST',
        mode: 'cors',
        headers: {
            'Content-Type': 'text/plain'
        },
        body,
    }
}



export namespace Client {

    export const USERNAME_MAX_LENGTH = 30;

    export const enum DE1_COMMS_CMDS {
        PING = "PING",
        REGISTER_USER = "RU",
    }

    export type CtorArgs = {
        readonly de1ServerAddress: string;
    };

    export namespace GetPosition {

        export const ON_ERROR = ((error): void => {
            alert("ERROR(" + error.code + "): " + error.message);
        }) as PositionErrorCallback;

        export const OPTIONS: PositionOptions = {
            maximumAge: 0, // never use cached version.
            enableHighAccuracy: true,
        };
    }
}


var client = new Client();
