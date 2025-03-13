# ARCHI Mk1

![alt text](https://github.com/ArchangelDesign/archi-controller/blob/master/design.PNG)


## Protocol

Updates from the machine (no request required)

### Current Temperature Update
```
| STX | P | V | CHAR | CHAR | CHAR | CHAR | ETX | BCC |
```
