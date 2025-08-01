#!/usr/bin/env php
<?php
// Simple PHP CGI script for testing
header('Content-Type: application/json');

$method = $_SERVER['REQUEST_METHOD'];
$path = $_SERVER['PATH_INFO'] ?? '/';

$response = array(
	'status' => 'success',
	'message' => 'CGI is working!',
	'method' => $method,
	'path' => $path,
	'timestamp' => date('Y-m-d H:i:s'),
	'server' => 'Webserv/1.0'
);

echo json_encode($response, JSON_PRETTY_PRINT);
?>
