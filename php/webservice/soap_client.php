<?php
  require_once "util.php";

	try {
		$username = get_current_user();
		$client = new SoapClient('http://server_name/apt/test/blog_webservice/sample.wsdl', 
									array( 'trace' => 1, 
											'exceptions' => 1,
											'cache_wsdl' => WSDL_CACHE_NONE,
										 )
								);
		
		
		$ret = $client->create_job($username, get_job(1));
		if($ret == 1) {
			echo "Successfully created job \n";
		} else {
			echo $ret . "\n";
		}
		
		$result = array();
		stdclassobject_to_array($result, $client->get_jobs($username));
		var_dump($result);
		
	
	} catch (SoapFault $e) {
		print "Error: " . $e->getMessage() . "\n";
	}	
?>
