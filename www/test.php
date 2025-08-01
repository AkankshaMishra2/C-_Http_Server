#!/usr/bin/env php
<?php
header('Content-Type: text/html');
?>
<!DOCTYPE html>
<html>
<head>
	<title>PHP CGI Test - Webserv</title>
	<style>
		body { font-family: Arial, sans-serif; margin: 40px; }
		.info { background: #e9ecef; padding: 20px; border-radius: 5px; margin: 10px 0; }
		.success { color: #28a745; }
		table { border-collapse: collapse; width: 100%; }
		th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
		th { background-color: #f2f2f2; }
	</style>
</head>
<body>
	<h1 class="success">✅ PHP CGI is working!</h1>

	<div class="info">
		<h3>Server Information:</h3>
		<p><strong>PHP Version:</strong> <?php echo phpversion(); ?></p>
		<p><strong>Request Method:</strong> <?php echo $_SERVER['REQUEST_METHOD'] ?? 'Unknown'; ?></p>
		<p><strong>Request URI:</strong> <?php echo $_SERVER['REQUEST_URI'] ?? 'Unknown'; ?></p>
		<p><strong>Server Software:</strong> Webserv/1.0</p>
		<p><strong>Current Time:</strong> <?php echo date('Y-m-d H:i:s'); ?></p>
	</div>

	<div class="info">
		<h3>Environment Variables:</h3>
		<table>
			<tr><th>Variable</th><th>Value</th></tr>
			<?php
			$env_vars = ['REQUEST_METHOD', 'PATH_INFO', 'QUERY_STRING', 'CONTENT_TYPE', 'CONTENT_LENGTH'];
			foreach ($env_vars as $var) {
				$value = $_SERVER[$var] ?? getenv($var) ?? 'Not set';
				echo "<tr><td>$var</td><td>$value</td></tr>";
			}
			?>
		</table>
	</div>

	<?php if ($_SERVER['REQUEST_METHOD'] == 'POST'): ?>
	<div class="info">
		<h3>POST Data:</h3>
		<pre><?php print_r($_POST); ?></pre>
		<h3>Raw Input:</h3>
		<pre><?php echo file_get_contents('php://input'); ?></pre>
	</div>
	<?php endif; ?>

	<div class="info">
		<h3>Test Form:</h3>
		<form method="post" action="">
			<p>
				<label>Name: <input type="text" name="name" value="Test User"></label>
			</p>
			<p>
				<label>Message: <input type="text" name="message" value="Hello from CGI!"></label>
			</p>
			<p>
				<button type="submit">Submit POST Request</button>
			</p>
		</form>
	</div>

	<p><a href="/">← Back to Home</a></p>
</body>
</html>
