const express = require('express');
const router  = express.Router();

const context = {};

router.get('/update', function (req, res) {
    console.info('GET /update');
    console.info(`query: ${JSON.stringify(req.query)}`);
    res.status(200);
    res.send('success');
});

router.get('/settings', function (req, res) {
    console.info('GET /settings');
    res.status(200);
    res.send({
            data: {
                brightness: 255,
                state: true,
                mode: "mode2",
                color: "100,100,100",
                states: "mode1,mode2,mode3"
            }
    });
});

router.get('/setcredentials', function (req, res) {
    console.info('GET /setcredentials');
    console.info(`query: ${JSON.stringify(req.query)}`);
    context.credentials = req.query;

    res.status(200);
    res.send('success');
});

router.get('/getcredentials', function (req, res) {
    console.info('GET /getcredentials');
    res.status(200);
    res.send(context.credentials);
});

router.get('/newauthpass', function (req, res) {
    console.info('GET /newauthpass');
    console.info(`query: ${JSON.stringify(req.query)}`);
    context.auth = req.query;

    res.status(200);
    res.send('success');
});

router.get('/resetdefault', function (req, res) {
    console.info('GET /resetdefault');

    res.status(200);
    res.send('success');
});

router.get('/reboot', function (req, res) {
    console.info('GET /reboot');

    res.status(200);
    res.send('success');
});

router.get('/reboot', function (req, res) {
    console.info('GET /reboot');

    res.status(200);
    res.send('success');
});

router.get('/connectedwifi', function (req, res) {
    console.info('GET /connectedwifi');

    res.status(200);
    res.send('2Smart');
});

router.get('/setwifi', function (req, res) {
    console.info('GET /setwifi');

    res.status(200);
    res.send('success');
});

router.get('/update', function (req, res) {
    console.info('GET /update');

    res.status(200);
    res.send('success');
});

router.post('/firmware/upload', function (req, res) {
    console.info('GET /firmware/upload');

    res.status(200);
    res.send('success');
});

router.get('/scan', (req, res) => {
    console.info('GET /scan');
    res.status(200);
    res.send({
        "WebbyLab_2.4":"3",
        "":"3",
        "EW11_4686":"0",
        "EW11_4A6E":"0",
        "Redmi\"":"0",
        "EW11_C55A":"0",
        "karbon":"3",
        "EW11_C7BA":"0",
        "Karbon_Guest":"3",
        "2smart":"3",
        "AGRII":"3",
        "DIRECT-56-HP M426 LaserJet":"3",
        "INTELLOGATE":"4",
        "V20":"3",
        "AGRII_GUEST":"3"
    });
});

router.get('/scan/v2', (req, res) => {
    console.info('GET /scan');
    res.status(200);
    res.send({ 
        "SSID" : {
            encType: "3",
            signal: "100"
        },
        "SSID_2" : {
            encType: "0",
            signal: "94" 
        }
    });
});

module.exports = router;