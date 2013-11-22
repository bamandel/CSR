<h1>Communication Platform</h1>
<p>Justin Carter<br>
8/1/2013</p>
<h2>Project Description</h2>
A communication platform used between Android devices and a server containing a MySQL database to transfer and store sensor data.
<h2>Requirements</h2>
<ul>
  <li>Android Device (I used a Nexus 1 on version 2.3.4)</li>
  <li>Amazon Web Services Account</li>
  <li>WAMP (Apache, MySQL, PHP)</li>
</ul>
<h2>Clean Installation and Usage</h2>
<h4>Make AWS Windows Server Instance</h4>
<ol>
  <li>Sign up for a free tier of AWS (Amazon Web Services). It requires a valid credit card number. http://aws.amazon.com/</li>
  <li>Use the EC2 service to launch a free micro instance of 64-bit Windows Server 2008 R2 Base. Don’t forget to create a Key Pair for the security of the server.</li>
  <li>Under Network & Security, click on Elastic IPs and allocate a new IP address to the instance (first IP is free).</li>
  <li>Once the server is running, setup a connection to it by right clicking on the instance and following the steps from the “Log in with your credentials” option.</li>
  <li>Make sure that you can remote into the server (Remote Desktop Connection will work)</li>
</ol>
<h4>Install Needed Software on AWS Server</h4>
<ol>
  <li>Remote into the server and download and install 64-bit WAMP. http://www.wampserver.com/en/</li>
  <li>Allow from all in the C:\wamp\bin\apache\Apache2.4.4\conf\httpd.conf file.</li>
  <li>Run the MySQL commands to create the database from the DatabaseSQL.txt file.</li>
  <li>Run the MySQL commands from the ADLInserts.txt file to insert the ADL codes.</li>
  <li>Put the handle_upload.php file in C:\wamp\www to allow the iFall app to write data to the database.</li>
</ol>
<h2>Using Current Installation</h2>
<h4>Using AWS Windows Server Instance</h4>
<ol>
  <li>Sign into Amazon Web Services using credentials located in the Google Drive.</li>
  <li>Once signed in, go to the EC2 page.</li>
  <li>In the top right corner, make sure that the location chosen is US East (N. Virginia).</li>
  <li>Click on running instances and make sure that the Windows Visualization Server is running.</li>
  <li>Click on Elastic IPs and associate the IP address with the running instance.</li>
  <li>Remote into the server as needed. WAMP is configured to start automatically on boot up of the server instance.</li>
</ol>
