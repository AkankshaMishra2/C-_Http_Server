console.log('Webserv JavaScript file loaded successfully!');

document.addEventListener('DOMContentLoaded', function() {
	console.log('DOM fully loaded');

	// Simple test function
	function testFunction() {
		alert('JavaScript is working on Webserv!');
	}

	// Add click event to any button if present
	const buttons = document.querySelectorAll('button');
	buttons.forEach(button => {
		button.addEventListener('click', testFunction);
	});
});
