import config from "mc/config";
import Timer from "timer";
import Button from "button";
import Digital from "pins/digital";

class Flash {
	constructor(options) {
		return new Button({
			...options,
			pin: 41,
			invert: true
		});
	}
}

globalThis.Host = Object.freeze({
	Button: {
		Default: Flash,
		Flash
	}
}, true);

export default function (done) {
	Digital.write(config.power_pin, 0);
	Timer.delay(500);

	done?.();
}