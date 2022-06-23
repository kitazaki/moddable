/*---
description: 
flags: [module]
---*/

const foo = { source:`
	let foo = 0;
	export default function() {
		return foo++;
	}
`};

function moduleMapHook(specifier) {
	if ((specifier == "foo") || (specifier == "bar"))
		return foo;
}

const c1 = new Compartment({print}, {}, { moduleMapHook });
const fooNS1 = await c1.import("foo");
const barNS1 = await c1.import("bar");

const c2 = new Compartment({print}, {}, { moduleMapHook });
const fooNS2 = await c2.import("foo");
const barNS2 = await c2.import("bar");

assert.sameValue(fooNS1.default(), 0, "fooNS1.default()");
assert.sameValue(barNS1.default(), 0, "barNS1.default()");
assert.sameValue(fooNS2.default(), 0, "fooNS2.default()");
assert.sameValue(barNS2.default(), 0, "barNS2.default()");
