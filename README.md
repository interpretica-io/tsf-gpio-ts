# tsf-gpio-ts

A Test Environment suite that exercises
[tsf-gpio](https://github.com/interpretica-io/tsf-gpio) (`tapi_gpio`)
against the agent it runs on - enumerating its GPIO chips over libgpiod.

| Test | What it checks |
|---|---|
| `list` | `tapi_gpio_list()` returns a self-consistent snapshot of the gpiochips; each chip logs (path, label, line count) and line 0 reads |

What is attached is the host's to decide, so the suite **asserts no chip
count**: an agent with no gpiochip is a clean pass (the RPC and
enumeration path still ran). Read-only - it never drives a line.

## Running it

Needs `test-environment` as a sibling directory.

- Native (real /dev/gpiochip*, requires libgpiod on the host):
  `./scripts/run.sh guess --cfg=localhost`
- In a container (libgpiod from the Dockerfile; no gpiochip visible, so
  `list` reports zero and passes): `./scripts/run.sh docker guess --cfg=localhost`

## Status

**Not yet run.** Written alongside tsf-gpio, not yet built or executed
(no TE toolchain was available at authoring time; tsf-gpio's libgpiod
usage is Linux-only and was not compiled on the authoring host). Expect
the ordinary first-build fixes.
