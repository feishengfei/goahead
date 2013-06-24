<html>
<meta http-equiv="Pragma" content="no-cache">
<script>
var flag="<% getGetUploadFlag(); %>";
if(flag == "1"){
	parent.location.href = "/local/advance/upload_process.asp";
}
else{
	parent.upload_message='Not a valid firmware.';
	self.location.href = '/local/advance/upload_firmware_gordon.asp';
}
</script>
</html>