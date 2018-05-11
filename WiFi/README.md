# WiFi Test

[TBD]

## HTTP GET Test

It tests HTTP GET http://www.coxlab.kr/.

## HTTPS GET Test

It tests HTTPS GET https://town.coxlab.kr/.
You can use openssl as below to get a CA root certificate.

```
openssl s_client -showcerts -connect town.coxlab.kr:443 </dev/null
```

The CA root certificate is the last one of the certificates chain.
