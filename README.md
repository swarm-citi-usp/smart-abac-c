A C implementation of the ABAC-them model.

Current status:

- [x] parse policies from json
- [x] helpers to instantiate requests
- [x] PDP: given a request and a set of policies, can make a decision
- [ ] attribute expansion

# Commands
- `make run`
- `make test`

# Dependencies

- [jansson](https://github.com/akheron/jansson), a C library for encoding, decoding and manipulating JSON data

# Benchmarks

- number of policies: m
- number of attributes / operations per policy: n

- m=2000 n=100, elapased time is 0.001493 (1.4 ms)

