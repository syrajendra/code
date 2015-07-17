php_webservice
==============
To create web service APIs with associative arrays(complexType) as arguments & return type. 

The web service creation has 3 parts

1. Server:
    One who provide web service using SOAPServer from PHP   

2. Client:
    One who access web service created PHP & Python clients

3. WSDL:
    Web Service Description Language is a W3C standard to describe web services.

Google for detailed info on web service.

In the process of developing web service major effort goes in writing WSDL file.  
There are open source tools which can generate WSDL file. But these tools are not good in handling complexType's.

Here is WSDL generator for complexType data. Input for this generator is PHP code with phpdoc style comments.

Source Description:

    web_api.php:       file has web API's defined with proper comments  
    template.wsdl:     file has template WSDL which is used by generator_wsdl.php  
    generate_wsdl.php: file has code to generate WSDL file  
    soap_server.php:   file has code to host web service  
    soap_client.php:   file has client code to access web service in PHP  
    soap_client.php:   file has client code to access web service in Python  
    util.php:          file has utility functions
    sample.wsdl        generated WSDL file
  

In util.php set below constants to correct values.

    define('WSDL_NAME', 'sample');
    define('WEB_SERVER_NAME', 'server_name');
    define('WSDL_PATH', 'apt/test/blog_webservice');
    define('WEB_SERVICE_NAME', 'webapis');

["server_name" to be your server name or IP address]
["apt/test/blog_webservice" assumed to be placed all these files in this folder]

To generate WSDL:

    $ php generate_wsdl.php

To run client:

    $ php soap_client.php
    $ python soap_client.py

Queries contact: rajendra.sy@gmail.com
