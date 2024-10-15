# DHCP-Stats

**Name:** dhcp-stats  
**Date:** 20.11.2023 
**Author:** Adrian Horvath (xhorva14)

## Brief

`dhcp-stats` is a tool designed for capturing and analyzing DHCP traffic on a network. The program assesses the percentage of allocated IP addresses relative to the total available addresses within a specified prefix.

## Files

- `src/dhcp-stats.cpp`: Main source code file.
- `src/dhcp-stats.h`: Header file containing function prototypes and structure definitions.
- `Makefile`: Makefile for building the program.
- `dhcp-stats.1`: Manual page for `dhcp-stats`.
- `manual.pdf`: PDF document containing detailed documentation.

## Usage

```bash
./dhcp-stats [-r <filename>] [-i <interface-name>] <ip-prefix> [ <ip-prefix> [ ... ] ]
```

Use either -r to read packets from a file or -i to capture live packets from a network interface.
Provide at least one IP prefix for analysis.

## Restrictions

The program relies on libraries for certain functionalities.
Ensure that the necessary permissions are granted for capturing live packets.
The program capturing only ACK and RELEASE packets which have right configuration accordin RFC 2131, program support also RELEASE packets, but does not include support for lease time.