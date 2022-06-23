/*---
description: 
flags: [async, module]
---*/

import {Request} from "http";

await $NETWORK.connected;

const request = new Request({host: "www.example.com", path: "/", response: String});
request.callback = function(message, value, etc) {
	if (Request.responseComplete == message)
		$DONE();
	else if (message < 0)
		$DONE(message)
}

$TESTMC.timeout(5_000);
