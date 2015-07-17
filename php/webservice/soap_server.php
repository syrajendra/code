<?php
  
	require_once 'web_api.php';
	require_once 'util.php';
	
	ini_set("soap.wsdl_cache_enabled", "0");
	try {
		$server = new SoapServer('sample.wsdl');
		$server->setClass('jobsC');	
		$server->handle();
		exit;
	} catch (SOAPFault $f) {
		log_msg(__FILE__, __FUNCTION__, __LINE__, $f->faultstring);
	}
?>
