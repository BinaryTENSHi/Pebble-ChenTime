Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('https://www.binarytenshi.com/pebble/chentime-configuration.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    Pebble.sendAppMessage(configuration,
      function(e) {
        console.log('Successfully delivered message with transactionId=' + e.data.transactionId);
      },
      function(e) {
        console.log('Unable to deliver message with transactionId=' + e.data.transactionId + ' Error is: ' + e.error.message);
      }
    );
  }
);
