<?php
  require_once "util.php";

	class jobsC {	
		
		/**
		* Function to create job.
		* 
		* @param string $user
		* @param array $job_description
		* @return integer
		*/
		function create_job($user, $job_description) {
			$job_array = array();						
			stdclassobject_to_array($job_array, $job_description);
			$msg = $user . " : " . array_to_string($job_array);
			log_msg(__FILE__, __FUNCTION__, __LINE__, $msg);		
			return 1;
		}

		/**
		* Function to read Jobs .
		* 
		* @param string $user
		* @return array
		*/
		function get_jobs($user) {
			$job = get_job(2);
			log_msg(__FILE__, __FUNCTION__, __LINE__, 'Returning: ' . array_to_string($job));
			return $job;
		}		
	}
?>
