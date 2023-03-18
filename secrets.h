#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "LokerRSTesting"                         //change this

const char AWS_IOT_ENDPOINT[] = "a2zv6zeb850fmp-ats.iot.ap-southeast-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUKfj0VXFYrqf09gS0VLJioNBSRZYwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDIxNTE4MjYy
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAPBpl55BbmNkZMoS0eyx
pRdy6sQMhLdkb6XYCIhzsOCdbH7haU1WnH/8ol/upep3Cz7axWMKXAZT0hol8Pik
iMnbpRJxVmZwI2WIBzob5LBlQ/PKNHSctoPkIMOxAjpfoUVYbLJonRPsynWLMLUr
DyyNK0Qcs/S7uuiMkrpyTq3Jb1tWHDPnIu98vv4hHMm/MZt6bJ1a4xGe/NIkamJ5
K+8BTwPqF2ebJ9cD3fbcc0/rrC92KpVzN8NpXYzTktwKOEK8EsQnPU2q5myCDj8v
HY5QjK2snNcwqXVM+TOEEESwGqFIdXUqNrReUS5ioPxN6zfsDNV7CELbZbuKvNy4
WUECAwEAAaNgMF4wHwYDVR0jBBgwFoAUhr7cn2xxv/meNQQCj/rnMhj1L/4wHQYD
VR0OBBYEFNJDGdJC0zhUFwciuy1DeEu2D4EPMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBky8/scdYCytOoTACvAhi0lyjY
cf+ic0QOTl7ujTMYdyuhKkiRkgnvMu5ihdmZg59vdAVOcp/FRvuPkegx8UGo0k/F
Aeqj7rAFnq6FV/FJOQ+I3j3leNpkdyymlfpdssTlLCS5S5CFlO80NIsTTiPAAIWu
QliBObd/6uwgOY+Lf+ZJMKfwKwbmrNezSL2nhvtgDoL1c5c9tKZCPi+f/0oiyLwn
n+d5A+mDSToBYfzbgsFcH75M6VfjayQjUwg8/JlSj8i5Ei4LJ7FBvMUv+I6wiX5h
Onxla+5HbJUD8Tq4XGcwg2xQ7SS6ujsc31LdsqlbJgaQJiJBYM+yyvDbLHxM
-----END CERTIFICATE-----
 

)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA8GmXnkFuY2RkyhLR7LGlF3LqxAyEt2RvpdgIiHOw4J1sfuFp
TVacf/yiX+6l6ncLPtrFYwpcBlPSGiXw+KSIydulEnFWZnAjZYgHOhvksGVD88o0
dJy2g+Qgw7ECOl+hRVhssmidE+zKdYswtSsPLI0rRByz9Lu66IySunJOrclvW1Yc
M+ci73y+/iEcyb8xm3psnVrjEZ780iRqYnkr7wFPA+oXZ5sn1wPd9txzT+usL3Yq
lXM3w2ldjNOS3Ao4QrwSxCc9TarmbIIOPy8djlCMrayc1zCpdUz5M4QQRLAaoUh1
dSo2tF5RLmKg/E3rN+wM1XsIQttlu4q83LhZQQIDAQABAoIBAQCcmvwkH9VLs4Mg
SwAqYlH7o2UgjlMJr8g2zTJJyKsItiONx0I0Gd5aVPRk721u/Vr698Gq0X2+tW1g
kSSWo39jlsvB5sh0dEnqgqDKjo77BQwty6M97zPJWJqrPywHYHIKXR55XUItT4tW
5HO3IDSDPfZYKSZknsGTaLbP81QjSZgJhsHMlkrpB6eoU/G+BEdU46mY5hzFjPWT
1YBtqGq2zRzMnTjdtz1JX30B/d0AFEjQfUTcPWHvFlogdrmiEoKHr/q/K93/RCs5
k1m+N/bNqw7vVLWuWdUvNQb2Ja2xGhv/WRrNe8QbTA5fWrhkJ8QIoDy1o8bZ8B37
crRoQPx9AoGBAP6j8KscdTjinOap98ZJ8RB3fFiGnZPVIJ/qXQ89teDYVVw3n71E
T/1xfqa4zWbDkKTfu5zbNjd8s4shY8wTQcHVjmePOFXiONe89RLHSJzRN1Qdzrdz
zPC/Luuvj5m53rjheS/tRscOSIQnfy1hWyJvv6trtCY8eO+qLiHHjWULAoGBAPGy
NFb+Dg5kxNUSnYDSxhR4BECFJnevbPjZAjkO3sqZOWj/jJA+0Tmi3eKLOTqgVylR
xF4whn5+0xP2oVBMkayaubE7gHU872B/pYY+abbDmYIW90LyTxcPtHzsw+v05IEb
zv6vBlIY5Qtv1YXmzd5D7FQ7wyBIfC/f3KI2fpJjAoGBAInEJpRc/Cq/qAW/1Xbp
UEwJaTP3+M8fktwSDzEMwRcZ0+zlhz399Iz5V31aJH7BtcMN7NIrY4TXqcfPU8/4
Zy44nSf4O5FzqC9IYws62FVHd/jBe7KTK0DhHsCSpk2UWySJkGhWpmFRn/gn6k2i
C41xCjDzP28kboWz++t4rE4LAoGBAI1mKTK/nO+UyoaxRk3ogZPWBv7MJw4cJk+L
fb5t0a+LZsfjwZNd/L+QZek0dD21Mmjs+kn0b6qvfcOPJR66PPtS5dInMmbkSFMX
OnpIJ0aSNkuiLfQPmG+wc3ImfhScB9b0suvS74q2H8HKIowc1FH1QNyivew0G9tS
R1zrLq9rAoGAXlcbzq59urgrwxQot8fOtAy+6XpsnWcs+VBfasGBgZcEGh+BbnqR
aXSwK6EifXezKDhIzgI6Lo4eWMAVR6dm01bnNJK9HgU0ziJSR1F3wQkar+OBg/av
acf+v2UqDw7AQTL1ktcW8Z4PvatyH8vdJ0qF4l75qJIgf4ZXR2xSNLk=
-----END RSA PRIVATE KEY-----

 
 
)KEY";
