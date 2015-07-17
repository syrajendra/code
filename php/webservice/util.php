<?php
  
	define('WSDL_NAME', 'sample');
	define('WEB_SERVER_NAME', 'server_name');
	define('WSDL_PATH', 'apt/test/blog_webservice');
	define('WEB_SERVICE_NAME', 'webapis');

	function get_task($name) {
		$task = array('name' => $name, 
					  'progress' => array('status' => 'Ongoing', 
					    					'data' => array('start' => 'start data', 'end' => 'end data')
							  			 )
					 );
		return $task;
	}

	function get_job($index) {
		
		$job = array('id' => $index,
					  'name' => 'job_name',
					  'task' => array()					  					
					  );	
		array_push($job['task'], get_task((string)$index . '-first_task'));
		array_push($job['task'], get_task((string)$index . '-second_task'));
		return $job;
	}
	
	function array_to_string($array_name) {
		ob_start();
		var_dump($array_name);
		$dump = ob_get_clean();
		return $dump;
	}

	function log_msg($file, $caller, $line, $msg) {		
		$full_msg = $file . ' ; ' . $caller . '() ; ' . $line . ' ; ' . $msg . "\n";
		error_log($full_msg, 3, '/var/www/apt/apt.log');
	}
	
	function stdclassobject_to_array(&$array, $dict) {
		foreach ($dict as $left => $right) {			
			if(!strcmp($left, 'item') && !strcmp(gettype($right), 'array')) { // 	means value is array				
				stdclassobject_to_array($array, $right);
			} else if(!strcmp($left, 'Map')) {
				stdclassobject_to_array($array, $right);
			} else if(!strcmp(gettype($right), 'object')) {
				if(isset($right->key) && isset($right->value)) {						
					$key	= $right->key;					
					$value 	= $right->value;
				} else {						
					$key 	= $left;
					$value	= $right;
				}				
				if(!strcmp(gettype($value), 'object')) {
					$array[$key] = array();
					stdclassobject_to_array($array[$key], $value);
				} else {
					$array[$key] = $value; 
				}					
			} else {
				$array[$left] = $right;
			}
		}							
	}
?>
