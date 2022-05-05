const path = require('path');
const express = require('express');
const router = require('./router');

const app = express();

app.disable('x-powered-by');
app.use(express.static(path.join(__dirname, 'build')));
app.use('/', router);

app.get('*', function (req, res) {
    res.sendFile(path.join(__dirname, 'build', 'index.html'));
});

app.listen(8000, function () {
    console.log(`Started on PORT 8000`)
});
