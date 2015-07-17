<?php
  require_once 'web_api.php';
	require_once 'util.php';

	$wsdl = array(  'TOOL_NAME'		=> WEB_SERVICE_NAME,
					'SERVER_NAME'  	=> WEB_SERVER_NAME,
					'PATH_TO_WSDL'  => WSDL_PATH,
					'WSDL_NAME'		=> WSDL_NAME,
					'SERVER_CODE' 	=> 'soap_server.php'					
				);	

	$data_types = array(	"string"		=> "xsd:string", 
							"bool"			=> "xsd:boolean", 
							"boolean"		=> "xsd:boolean",
							"int"			=> "xsd:integer", 
							"integer"		=> "xsd:integer", 
							"double"		=> "xsd:double", 
							"float"			=> "xsd:float", 
							"number"		=> "xsd:float",
							"datetime"		=> "xsd:datetime",
							"resource"		=> "xsd:anyType", 
							"mixed"			=> "xsd:anyType", 
							"unknown"		=> "xsd:anyType", 
							"unknown_type"	=> "xsd:anyType", 
							"anytype"		=> "xsd:anyType",
							"array"			=> "tns:array_of_key_value_data"
						);

	$func_return = "\t\t<wsdl:part name=\"{FUNC_NAME}_result\" type=\"{PARAM_TYPE}\"/>";
	$func_arg = "\t\t<wsdl:part name=\"{PARAM_NAME}\" type=\"{PARAM_TYPE}\"/>";

	$message = array("\t<wsdl:message name=\"{FUNC_NAME}_request\">",
					 "\t\t{FUNCTION_ARGUMENTS}",		
					 "\t</wsdl:message>",
					 "",
					 "\t<wsdl:message name=\"{FUNC_NAME}_response\">",
					 "\t\t{FUNCTION_RETURN}",
					 "\t</wsdl:message>"
					);

	$operation = array("\t\t<wsdl:operation name=\"{FUNC_NAME}\">",
						"\t\t\t<wsdl:input message=\"tns:{FUNC_NAME}_request\"/>",
						"\t\t\t<wsdl:output message=\"tns:{FUNC_NAME}_response\"/>",
						"\t\t</wsdl:operation>"
					);

	$soap_action = array("\t\t<wsdl:operation name=\"{FUNC_NAME}\">",
						 "\t\t\t<soap:operation soapAction=\"urn:localhost-{FUNC_NAME}#{FUNC_NAME}\"/>",
						 "\t\t\t<wsdl:input>",
						 "\t\t\t\t<soap:body namespace=\"urn:localhost-{FUNC_NAME}\" use=\"literal\" />",
						 "\t\t\t</wsdl:input>",
						 "\t\t\t<wsdl:output>",
						 "\t\t\t\t<soap:body namespace=\"urn:localhost-{FUNC_NAME}\" use=\"literal\" />",
						 "\t\t\t</wsdl:output>",
						 "\t\t</wsdl:operation>"
						);

	function wsdl_token_replace($line, $wsdl) {			
		foreach($wsdl as $key => $value) { 	
			$old_str = '{' . $key . '}';
			$new_str = $value;			
			$line = str_replace($old_str, $new_str, $line);			
		}
		return $line;
	}

	function create_wsdl_messages($obj, $fp) {
		GLOBAL $wsdl, $data_types, $func_arg, $func_return, $message;		
		$methods = get_class_methods($obj);
		foreach($methods as $method) {			
			$rm = new ReflectionMethod($obj, $method);			
			if($rm->isConstructor() || $rm->isDestructor()) continue;
			//echo "Function Name: " . $method . "\n";
			$new_wsdl = array_merge($wsdl, array('FUNC_NAME' => $method));
			if(!$comment = $rm->getDocComment()) {
				print "Error: Failed to find comments for method - " . $method . "\n";
				exit;
			}

			$msg_arguments = '';
			//echo "\nComment: " . $comment . "\n";			
			$params = $rm->getParameters();
			foreach ($params as $param) {	    		
				$p_name = $param->getName();
				$pattern = '/\@param.*?\$' . $p_name . '\r\n/';
				//echo 'Pattern: ' . $pattern . "\n";
				$matches = array();
				preg_match($pattern, $comment, $matches);
				if(empty($matches) || (count($matches) > 1)) {
					print "Error: Failed to find type of argument - " . $p_name . "\n";
					exit;
				} else {	    			
					$replace = array('@param', '$' . $p_name, "\n", "\r", " ");	    			
					$p_type  = str_replace($replace, "", $matches[0]);
					$p_new_type= $data_types[$p_type];
					//echo "\t" . $p_new_type . " = " . $p_name . "\n";
					$msg_arguments .= wsdl_token_replace($func_arg, array('PARAM_NAME' => $p_name, 'PARAM_TYPE' => $p_new_type)) . "\n";					
				}
			}

			$msg_return = '';
			$matches = array();
			$pattern = '/\@return.*?\r\n/';
			preg_match($pattern, $comment, $matches);
			if(empty($matches) || (count($matches) > 1)) {
				print "Error: Failed to find return type  \n";
				exit;
			} else {	    			
				$replace = array('@return', "\n", "\r", " ");	    			
				$r_type  = str_replace($replace, "", $matches[0]);
				$r_new_type= $data_types[$r_type];
				//echo "\treturn " . $r_new_type . "\n";
				$msg_return .= wsdl_token_replace($func_return, array('FUNC_NAME' => $method, 'PARAM_TYPE' => $r_new_type)) . "\n";	
			}
			foreach ($message as $line) {
				//print "[" . $line . "]" . "\n";
				if(strpos($line, '{FUNCTION_ARGUMENTS}') !== false) {				
					fwrite($fp, $msg_arguments);
				} else if(strpos($line, '{FUNCTION_RETURN}') !== false) {				
					fwrite($fp, $msg_return);
				} else {
					$line = wsdl_token_replace($line, $new_wsdl);
					fwrite($fp, $line . "\n");	
				}				
			}		
			fwrite($fp, "\n");
		}
	}

	function create_wsdl_operations($obj, $fp) {
		GLOBAL $operation;		
		$methods = get_class_methods($obj);
		foreach($methods as $method) {
			if(!strcmp('__construct', $method) || !strcmp('__destruct',  $method)) continue;
			foreach ($operation as $line) {
				$line = wsdl_token_replace($line, array('FUNC_NAME' => $method));
				fwrite($fp, $line . "\n");
			}
			fwrite($fp, "\n");
		}
	}

	function create_wsdl_soap_actions($obj, $fp) {
		GLOBAL $soap_action;		
		$methods = get_class_methods($obj);
		foreach($methods as $method) {
			if(!strcmp('__construct', $method) || !strcmp('__destruct',  $method)) continue;
			foreach ($soap_action as $line) {
				$line = wsdl_token_replace($line, array('FUNC_NAME' => $method));
				fwrite($fp, $line . "\n");
			}
			fwrite($fp, "\n");
		}
	}

	function create_wsdl($obj) {
		GLOBAL $wsdl;
		$template = explode("\n", file_get_contents('template.wsdl'));		
		$fp = fopen($wsdl['WSDL_NAME'] . '.wsdl', "w");
		foreach ($template as $line) {
			//print "[" . $line . "]" . "\n";
			if(strpos($line, '{WSDL_MESSAGES}') !== false) {							
				create_wsdl_messages($obj, $fp);
			} else if(strpos($line, '{WSDL_OPERATIONS}') !== false) {
				create_wsdl_operations($obj, $fp);
			} else if(strpos($line, '{WSDL_SOAP_ACTIONS}') !== false) {	
				create_wsdl_soap_actions($obj, $fp);
			} else {
				$line = wsdl_token_replace($line, $wsdl);
				fwrite($fp, $line . "\n");			
			}
		}
	}
	
	$job = new jobsC();
	create_wsdl($job);
	print "Successfully created WSDL - " . $wsdl['WSDL_NAME'] .".wsdl \n";
?>
