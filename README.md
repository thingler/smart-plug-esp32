# Thinger Smart Plug ESP32

Thingler Smart Plug ESP32 is the software controlling the ESP32 microcontroller for a smart plug device. The software is written in C and using the esp-idf library. The microcontroller will connect with [AWS IoT](https://aws.amazon.com/iot/) and the functionallty of the smart plug can be controlled with help of the AWS IoT service.

## Getting started

### Requirements

* ESP32 development, for example [Thingler ESP32-PICO-D4 Module](https://github.com/thingler/esp32-pico-d4-module)

* Follow Espressif Systems [Getting started](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) instructions to install esp-idf library

* [Amazon Web Services (AWS)](https://aws.amazon.com/) account

### Configure AWS IoT

* Sign into your AWS account

* Select the region you want to work in, e.g. N. Virginia (us-east-1) from the right side on the top menu

* Click in Services in the top meny and select [IoT Core](https://eu-west-1.console.aws.amazon.com/iot)

* Create a new thing from Manage --> Things --> Register a thing

* Create a single thing

* Give a name an hit next
* Click on "create certificate"

* Save all certificates including the root CA for AWS IoT and hit activate

* Click Done

* Click on your new thing --> interact and copy the HTTPS rest API enpoint (e.g. someid-ats.iot.us-east-1.amazonaws.com) and store it for later

* From the left hand meny click Secure --> Polices --> Create policy

* Name the policy, enter `iot:*` for the action field, `*` for Resource ARN, select the `Allow` checkbox and hit Create

* Attach the policy to the certificate from Secure --> Certificates, click the three dots on the certificate and select Attatch policy. Select the newly created policy and hit attach

### Configure ESP32

Start by cloning the project with submodules

```
git clone --recurse-submodules https://github.com/thingler/smart-plug-esp32.git
```

Next copy the certificates that was saved from the AWS console into the directory, `smart-plug-esp32/main/certs`. The certificates has to be named, `aws-root-ca.pem`, `certificate.pem.crt` and `private.pem.key`.

### Compile

Now the software is ready to be compiled. Make sure that the ESP-IDF tools variables has been exported with:

```
. $HOME/esp/esp-idf/export.sh
```

Now it is possible to configure the application with:

```
idf.py menuconfig
```

* Select `Thingler Smart Plug Configuration` and edit the values for `WiFi SSID` and `WiFi Password`

* Select `Component config` --> `Amazon Web Services IoT Platform` and edit the value for `AWS IoT Endpoint Hostname`. Use the enpoint that was copied in the steps for configuring AWS IoT.

* Hit `S` for saving the configuration and hit `Q` for exiting the configurations

Now compile the software with the following command

```
idf.py build
```

### Flash ESP32

The last step is to flash the software to the ESP32 Module. Connect your ESP32 programmer and flash the software with the following command

```
idf.py flash
```

It's also possible to read the serial communication from the ESP32 microcontroller with

```
idf.py flash monitor
```

### Test ESP32

The easiest way to test the communication between AWS IoT and the ESP32 microcontroller is to

* Sign into your AWS account

* Click in Services in the top meny and select [IoT Core](https://eu-west-1.console.aws.amazon.com/iot)

* Click `Test` from the left hand menu

* Fill in `smart/plug` in the subscription topic filed and click "Subscribe to topic"

* Power up ESP32. The message "Thing is connected" should appear after a short while in the test window on AWS.

* The defined `GPIO_SWITCH` pin in the software can be turned on by writing `on` in the test window and hit "Publish to topic"

* The same pin can be turned off by writing `off` and publishing it to the topic

## License

Released under [GNU/GPL Version 3](LICENSE)
