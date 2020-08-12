A C implementation of the ABAC-them model.

Current status:

- [x] helpers to instantiate attributes, requests, permissions, graph
- [x] PDP: given a request and a set of policies, can make a decision
- [x] attribute expansion using depth-first search
- [ ] load policies from json/cbor

# Commands
- `make run`
- `make test`

# Platforms
- Unix
- Mbed
- ESP32

# Benchmarks

### Laptop Intel quad-core i7 1.8 GHz, 8 GB RAM
The time taken to authorize 1 request against 6 policies, 3000 times, was 0.811000 ms
The time taken to authorize 1 request against 3000 policies was 0.221000 ms

### Labrador, ARM quad-core 1.3 GHz, 2 GB RAM
The time taken to authorize 1 request against 6 policies, 3000 times, was 5.733000 ms
The time taken to authorize 1 request against 3000 policies was 0.964000 ms

### ESP32, Xtensa dual-core, 240MHz, 520 KB RAM
The time taken to authorize 1 request against 6 policies, 3000 times, was 53 ms
The time taken to authorize 1 request against 3000 policies was 18 ms
