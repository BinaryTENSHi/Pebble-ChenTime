Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('https://www.binarytenshi.com/pebble/chentime-configuration.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    console.log('Configuration window returned: ', JSON.stringify(configuration));
  }
);
