<?php

  function construct_inner_tab_contents($key, $list) {
		$content = '';
		$i = 0;	
		$is_active = '';
		foreach ($list as $item) {
			$i++;
			if($i == 1) {
				$is_active = ' active';			
			} else {
				$is_active = '';
			}
			$tag_id  = $key . '-' . $item;			
			$content = $content . '<div class="tab-pane' . $is_active .  '" id="innertab-' . $tag_id .'">' . PHP_EOL 								
								. '<p> Here add whatever  you want to add id="innertab-' . $tag_id .'" </p>' . PHP_EOL 
								. '</div>' . PHP_EOL;
      	}
      	return $content;
	}

	function construct_inner_tabs($key, $list) {
		$tab = '<div class="tabbable">' . PHP_EOL .'<ul class="nav nav-tabs">' . PHP_EOL;
		$i = 0;	
		foreach ($list as $item) {
				$i++;								
				if($i == 1) {
					$tab = $tab . '<li class="active">' . PHP_EOL;
				} else {
					$tab = $tab . '<li>' . PHP_EOL;
				}
				$tag_id  = $key . '-' . $item;
				$tab = $tab . '<a href="#innertab-' . $tag_id  . '" data-toggle="tab" style="font-weight: bold;">' 
							. $item . '</a>' . PHP_EOL . '</li>';
		}
		$tab = $tab . '</ul>' . PHP_EOL;
		$tab = $tab . '<div class="tab-content">' . PHP_EOL 
					. construct_inner_tab_contents($key, $list) . PHP_EOL 
					. '</div>' . PHP_EOL . '</div>';
		return $tab;	
	}

	function construct_outer_tab_contents($outlist, $inlist) {
		$content = '';
		$i = 0;	
		$is_active = '';
		foreach ($outlist as $key) {
			$i++;
			if($i == 1) {
				$is_active = 'active';			
			} else {
				$is_active = '';
			}
			$content = $content . '<div class="tab-pane ' . $is_active .  '" id="outertab-' . $key . '">' . PHP_EOL 
								. construct_inner_tabs($key, $inlist[$key]) . PHP_EOL 
								. '</div>' . PHP_EOL;
      	}
      	return $content;
	}
	
	function construct_outer_tabs($outlist, $inlist) {
		$tab = '<div class="tabbable">' . PHP_EOL .'<ul class="nav nav-tabs">' . PHP_EOL;
		$i = 0;	
		foreach ($outlist as $item) {
				$i++;								
				if($i == 1) {
					$tab = $tab . '<li class="active">' . PHP_EOL;
				} else {
					$tab = $tab . '<li>' . PHP_EOL;
				}				
				$tab = $tab . '<a href="#outertab-' . $item 
							. '" data-toggle="tab" style="margin-top: -20px; font-weight:bold">' 
							. strtoupper($item) . '</a>	' . PHP_EOL . '</li>';
		}
		$tab = $tab . '</ul>' . PHP_EOL;
		$tab = $tab . '<div class="tab-content">' . PHP_EOL 
					. construct_outer_tab_contents($outlist, $inlist) . PHP_EOL  
					. '</div>' . PHP_EOL . '</div>';
		return $tab;	
	}
	
?>

<!DOCTYPE html>
<html lang="en">
	<head>
		<title>	Tabs </title>
		<meta charset="UTF-8">
		<meta name="description" content="tabs test">
		<meta name="author" content="Rajendra S Y">
		<meta name="keywords" content="tabs inside tab" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0">

		<link href="/apt/bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">				
		<link href="/apt/bootstrap/css/bootstrap-responsive.min.css" rel="stylesheet" media="screen">			
		<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
		<script src="/apt/bootstrap/js/bootstrap.min.js"></script>
	</head>
	<body>
		<div class="container-fluid">
  			<div class="row-fluid">
  				<div class="control-group">	
					<legend>Create Tabs</legend>
					</br> </br>
					<?php
						$outlist = array('dog', 'cat', 'cow');			
						$inlist = array();
						$inlist['dog'] = array('blackDog', 'whiteDog', 'pinkDog');
						$inlist['cat'] = array('blackCat', 'brownCat');
						$inlist['cow'] = array('whiteCow');
						echo construct_outer_tabs($outlist, $inlist);
					?>
				</div>
  			</div>
  		</div>
	</body>
</html>
