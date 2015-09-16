   mn = "";
   msn = ""; 
   analogPinOffset = 14;

   $(document).ready(function () {

        var user = readCookie("login");        

        if (user == null) {
            if (window.location.href.toLowerCase().indexOf("relays") == -1)
            {
            window.location.href = "relays.htm";
            return;
            }
            else
            {
                return;
            }
        }

        createCookie("login", user, 2);

        buttonsStorageName = user + "_buttons";
        modesStorageName = user + "_modes";
        relaysStorageName = user + "_relays";
        alarmsStorageName = user + "_alarms";
        rulesStorageName = user + "_rules";
        motionSensorsStorageName = user + "_motion";
        soundSensorsStorageName = user + "_sound";
        configStorageName = user + "_config";

        var buttons = deserializeJson(buttonsStorageName);
        var motionSensors = deserializeJson(motionSensorsStorageName);
        var relays = deserializeJson(relaysStorageName);
        var modes = deserializeJson(modesStorageName);
        var alarms = deserializeJson(alarmsStorageName);
        var rules = deserializeJson(rulesStorageName);
        var soundSensors = deserializeJson(soundSensorsStorageName);
        var config = deserializeJson(configStorageName);

        if (relays != null && self.relaysView != null) {
            self.relaysView(relays);
        }

        if (modes != null && self.modes != null) {
            self.modes(modes);
        }

        if (motionSensors != null && self.motionSensors != null) {
            self.motionSensors(motionSensors);
        }

        if (soundSensors != null && self.soundSensors != null) {
            self.soundSensors(soundSensors);
        }

        if (buttons != null && self.buttons != null) {
            self.buttons(buttons);
        }

        if (relays != null && self.relays != null) {
            self.relays(relays);
        }

        if (alarms != null && self.alarms != null) {
            self.alarms(alarms);
        }

        if (rules != null && self.rules != null) {
            self.rules(rules);
        }

        if (config != null && self.ipAddress != null) {
            self.ipAddress(config.ipAddress);
        }
    });

    function getObjectFromId(array, id) {
         for (var i = 0; i < array.length; i++) {
            if (array[i].id == id)
            {
                return true;
            }
         };
         return false;
    };

    function resetConditionsAndActions() {
        for (var i = 0; i < self.modes().length; i++) {
            self.modes()[i].isCondition = false;
            self.modes()[i].isAction = false;
            self.modes()[i].cs = -1;
            self.modes()[i].as = -1;
            self.modes()[i].waitSeconds = 0;
            self.modes()[i].waitMinutes = 0;
            self.modes()[i].waitHours = 0;
            self.modes()[i].durationSeconds = 0;
            self.modes()[i].durationMinutes = 0;
            self.modes()[i].durationHours = 0;
        };

        for (var i = 0; i < self.alarms().length; i++) {
            self.alarms()[i].isCondition = false;
            self.alarms()[i].isAction = false;
            self.alarms()[i].cs = -1;
            self.alarms()[i].as = -1;
            self.alarms()[i].waitSeconds = 0;
            self.alarms()[i].waitMinutes = 0;
            self.alarms()[i].waitHours = 0;
            self.alarms()[i].durationSeconds = 0;
            self.alarms()[i].durationMinutes = 0;
            self.alarms()[i].durationHours = 0;
        };

        for (var i = 0; i < self.relays().length; i++) {
            self.relays()[i].isCondition = false;
            self.relays()[i].isAction = false;
            self.relays()[i].cs = -1;
            self.relays()[i].as = -1;
            self.relays()[i].waitSeconds = 0;
            self.relays()[i].waitMinutes = 0;
            self.relays()[i].waitHours = 0;
            self.relays()[i].durationSeconds = 0;
            self.relays()[i].durationMinutes = 0;
            self.relays()[i].durationHours = 0;
        }
    };

    function config(ipAddress) {
        this.ipAddress = ipAddress;
    };

    function getConfig() {
        var user = readCookie("login");
        var userConfig = user + "_config";
        
        return deserializeJson(userConfig);
    };

    function getId() {
        var user = readCookie("login");
        var userId = user + "_id";
        var id = localStorage.getItem(userId);

        if (isNumber(id)) {
            id++;
            localStorage.setItem(userId, id);
        }
        else {
            id = 1;
            localStorage.setItem(userId, id);
        }

        return id;
    };

    function decreaseId() {
        var user = readCookie("login");
        var userId = user + "_id";
        var id = localStorage.getItem(userId);

        if (isNumber(id)) {
            id--;
            localStorage.setItem(userId, id);
        }
        else {
            id = 0;
            localStorage.setItem(userId, id);
        }
        return id;
    };

    function resetUI() {
        $(".waitTime").find('input').val(0);
        $(".durationTime").find('input').val(0);
        $("input:checked").trigger('click');
        $(".onButton").removeClass("onToggled");
        $(".offButton").removeClass("offToggled");
        $(".alarmLevel").val('');
        self.lastEventClicked.removeClass("active");
        self.lastEventClicked = null;
    };

    function toSeconds(hours, minutes, seconds) {  
        var number = 0;

        number += parseInt(hours) * 3600;
        number += parseInt(minutes) * 60;
        number += parseInt(seconds);

        return number;
    };
    
    function isNumber (o) {
      return ! isNaN (o-0) && o != null;
    };

    function removeRule(rule) {
        self.rules.remove(rule);
        saveChanges();
    };

    function addClockEvent() {
        if (validateTimeInput() == false) {
            alert("Clock event is not in format: hh:mm");

            return;
        }
        var time = $('#timeInput').val();
        time = time.replace(':', '');
	time = parseInt(time);

        self.eventToAdd = new event("Clock", time, 0, "Clock." + $('#timeInput').val()); // Id 0 is reserved for the clock
        return true;
    };

    function addMotionEvent(motionSensor) {
        self.eventToAdd = new event(motionSensor.msn, 1, motionSensor.id, motionSensor.msn + ".motion");
        return true;
    };

    function addSoundEvent(soundSensor) {
        self.eventToAdd = new event(soundSensor.sn, 2, soundSensor.id, soundSensor.sn + ".double_clap");
        return true;
    };

    function addSinglePushEvent(button) {
        self.eventToAdd = new event(button.bn, 1, button.id, button.bn + ".single_push");
        return true;
    };

    function addDoublePushEvent(button) {
        self.eventToAdd = new event(button.bn, 2, button.id, button.bn + ".double_push");
        return true;
    };

    function addLongPushEvent(button) {
        self.eventToAdd = new event(button.bn, 3, button.id, button.bn + ".long_push");
        return true;
    };

    function setConditionProperty(object) {
        object.isCondition = !object.isCondition;
        return true;
    };

    function setActionProperty(object) {
        object.isAction = !object.isAction;
        return true;
    };

    function addConditionOnToObject(object) {
        object.cs = 1;
    };

    function addConditionOffToObject(object) {
        object.cs = 0;
    };

    function addActionOnToObject(object) {
        object.as = 1;
    };

    function addActionOffToObject(object) {
        object.as = 0;
    };

    function event(name, eventType, id, eventString) {
        this.en = name;
        this.et = eventType;
        this.id = id;
        this.eventString = eventString;
    }; 

    function rule(event, conditions, actions) {
        this.e = event;
        this.c = ko.observableArray(conditions);
        this.a = ko.observableArray(actions);
    };

    function condition(name, state, id) {
        this.cn = name;
        this.s = state;
        this.id = id;
    };

    function action(name, state, wait, duration, id) {
        this.an = name;
        this.s = state;
        this.w = wait;
        this.d = duration;
        this.id = id;
    };

    function relay(name, inputPin, id) {
        this.rn = ko.observable(name);
        this.p = ko.observable(inputPin);
        this.isCondition = false;
        this.isAction = false;
        this.waitSeconds = 0;
        this.waitMinutes = 0;
        this.waitHours = 0;
        this.durationSeconds = 0;
        this.durationMinutes = 0;
        this.durationHours = 0;
        this.w = 0;
        this.d = 0;
        this.as = 0;
        this.cs = 0;
        this.id = id;
    };

    function motionSensor(name, inputPin, id)
    {
        this.msn = ko.observable(name);
        this.p = ko.observable(inputPin);
        this.id = id;
    };

    function soundSensor(name, inputPin, id)
    {
        this.sn = ko.observable(name);
        this.p = ko.observable(inputPin);
        this.id = id;
    };

    function alarm(name, outputPin, id) {
        this.an = ko.observable(name);
        this.p = ko.observable(outputPin);
        this.isCondition = false;
        this.isAction = false;
        this.waitSeconds = 0;
        this.waitMinutes = 0;
        this.waitHours = 0;
        this.durationSeconds = 0;
        this.durationMinutes = 0;
        this.durationHours = 0;
        this.w = 0;
        this.d = 0;
        this.as = 0;
        this.cs = 0;
        this.id = id;
    }; 

    function mode(name, id) {
        this.mn = ko.observable(name);
        this.isCondition = false;
        this.isAction = false;
        this.waitSeconds = 0;
        this.waitMinutes = 0;
        this.waitHours = 0;
        this.durationSeconds = 0;
        this.durationMinutes = 0;
        this.durationHours = 0;
        this.w = 0;
        this.d = 0;
        this.as = 0;
        this.cs = 0;
        this.id = id;
    }

    function config(ipAddress) {
        this.ipAddress = ko.observable(ipAddress);
    };

    function button(name, inputPin, id)
    {
        this.bn = ko.observable(name);
        this.p = ko.observable(inputPin);
        this.id = id;
    };

    function buildPostStr(type, data) {
        return "t=" + type + "&d=" + data;
    };

    function combinePostString(responseText) {
        var postStr = "";
        for (var i = 0; i < responseText.length; i++) 
        {
            if (i == responseText.length-1)
            {
                postStr += buildPostStr(responseText[i].type, responseText[i].data);
            }
            else 
            {
                postStr += buildPostStr(responseText[i].type, responseText[i].data) + "&"
            }
        };
        return postStr;
    }

    function numericObservable(initialValue) {
        var _actual = ko.observable(initialValue);

        var result = ko.dependentObservable({
            read: function () {
                return _actual();
            },
            write: function (newValue) {
                var parsedValue = parseFloat(newValue);
                _actual(isNaN(parsedValue) ? newValue : parsedValue);
            }
        });

        return result;
    }

    function validatePin(evt) {
        var theEvent = evt || window.event;
        var key = theEvent.keyCode || theEvent.which;
        key = String.fromCharCode(key);
        var regex = /[0-9]|\./;
        if (!regex.test(key)) {
            theEvent.returnValue = false;
            if (theEvent.preventDefault) theEvent.preventDefault();
        }
    }

    function createCookie(name, value, days) {
        if (days) {
            var date = new Date();
            date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
            var expires = "; expires=" + date.toGMTString();
        }
        else var expires = "";
        document.cookie = name + "=" + value + expires + "; path=/";
    }

    function readCookie(name) {
        var nameEQ = name + "=";
        var ca = document.cookie.split(';');
        for (var i = 0; i < ca.length; i++) {
            var c = ca[i];
            while (c.charAt(0) == ' ') c = c.substring(1, c.length);
            if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
        }
        return null;
    }

    function login() {
        var password = document.getElementById("passwordInput").value.toLowerCase().trim();
        var username = document.getElementById("usernameInput").value.toLowerCase().trim();
        var users = [];

        $.ajax({
          url: "users.json",
          dataType: "json",
          async: false,
          success: function(response) {
            users = response;
          }
        });

        for (var i = 0; i < users.length; i++) {
            if (users[i].username == username && users[i].password == password)
            {
                $('#loginBox').remove();

                if (!readCookie("login")) {
                    createCookie("login", username, 2);
                }
                return; 
            }
        };

        password = ""; 
        username = "";
        alert("Username and password did not match.");
    }

    getDomObject = function(item) { return $("#item_" + item.id); }


    function saveJson(name, object) { 
        var json_text = ko.toJSON(object);

        localStorage.setItem(name, json_text);
    };

    function deserializeJson(name) {
        var retrievedData = localStorage.getItem(name);

        if (retrievedData) {
            return JSON.parse(retrievedData);
        }
    };

    function readStorage(name) {
        var retrievedData = localStorage.getItem(name);

        if (retrievedData) {
            return retrievedData;
        }
    };

    $('.dropdown-menu').on('click', function (e) {
        e.stopPropagation();
    });

    $(function () {
        $("#loadFromCU").on("click", function (e) {
            if (!confirm('Do you wish to load configurations from the CU?'))
            {
                return;
            }

            var config = getConfig();  

            $.ajax({
            type: 'GET',
            url: "http://" + config.ipAddress + "/GetConfig",
            async: false,
            timeout: 3000,
                success: function (responseText) {
                    data = responseText.split('\n');

                    var pythonCall = $.ajax({
                        type: 'POST',
                        url: "tp2json.py",
                        data:
                        {
                            BL: data[0],
                            REL: data[1],
                            MSL: data[2], 
                            ML: data[3],
                            AL: data[4],
                            SSL: data[5],
                            RUL: data[6],
                        },
                        async: false,
                        success: function () {
                            var json = pythonCall.responseText;
                        }
                    });
                }
            });
        });
    });

    $(function () {
        $("#loadFromLocalFile").on("click", function (e) {
            e.preventDefault();
            $("#upload:hidden").trigger('click');
        });
    });

    function handleFileSelect(evt) {
       // Create a reader object
        var reader = new FileReader();
        if (evt.target.files.length) {
            var textFile = evt.target.files[0];
            // Read the file
            reader.readAsText(textFile);
            // When it's loaded, process it
            $(reader).on('load', processFile);
        } else {
            alert('Please upload a file before continuing')
        } 
    };

    function processFile(e)
    {
        if (confirm('Are you sure you wish to continue loading these configurations?'))
        {
            var json_string = e.target.result;
            var config = JSON.parse(json_string);

            if (config[0] != null) { localStorage.setItem(rulesStorageName, config[0]); }
            if (config[1] != null) { localStorage.setItem(buttonsStorageName, config[1]); }
            if (config[2] != null) { localStorage.setItem(relaysStorageName, config[2]); }
            if (config[3] != null) { localStorage.setItem(soundSensorsStorageName, config[3]); }
            if (config[4] != null) { localStorage.setItem(motionSensorsStorageName, config[4]); }
            if (config[5] != null) { localStorage.setItem(modesStorageName, config[5]); }
            if (config[6] != null) { localStorage.setItem(alarmsStorageName, config[6]); }
            if (config[7] != null) { localStorage.setItem(configStorageName, config[7]); }

            // Get current id after Loading config file
            var id = 0;
            for (var i = 1; i < config.length; i++) {
                if (config[i] != null)
                {         
                    id += JSON.parse(config[i]).length;
                }
            };

            var user = readCookie("login");
            var userId = user + "_id";
            localStorage.setItem(userId, id);

            window.location.reload()
        }
    };

    document.getElementById('upload').addEventListener('change', handleFileSelect, false);

    //Upload config to Arduino
    $(function () {
        $("#uploadConfigurations").on("click", function (e) {
            if (!confirm('Do you wish upload these configurations to the CU?')) {
                return;
            }

            var postStr = "";
                


            $.ajax({
                type: 'POST',
                url: "json2tp.py",
                data:
                {
                    BL: readStorage(buttonsStorageName),
                    REL: readStorage(relaysStorageName),
                    MSL: readStorage(motionSensorsStorageName),
                    ML: readStorage(modesStorageName),
                    AL: readStorage(alarmsStorageName),
                    SSL: readStorage(soundSensorsStorageName),
                    RUL: readStorage(rulesStorageName)
                },
                dataType: "json",
                async: true,
                success: function (responseText) {
                    var config = getConfig();  
                    postStr = combinePostString(responseText);

                    $.ajax({
                    type: 'POST',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    url: "http://" + config.ipAddress + "/NewConfig",
                    data: postStr,
                    async: true,
                    dataType: "application/x-www-form-urlencoded",
                    })
                }
            });
        });
    });

    //Save config to Client
    $(function () {
        $("#saveConfigurations").on("click", function (e) {
            e.preventDefault(); // cancel the link itself

            var config = []

            config[0] = readStorage(rulesStorageName);
            config[1] = readStorage(buttonsStorageName);
            config[2] = readStorage(relaysStorageName);
            config[3] = readStorage(soundSensorsStorageName);
            config[4] = readStorage(motionSensorsStorageName);
            config[5] = readStorage(modesStorageName);
            config[6] = readStorage(alarmsStorageName);
            config[7] = readStorage(configStorageName);

            if (confirm('Do you wish to save the configurations locally?')) {
                var filenameDisplay = "Configurations.json";
                var pom = document.createElement('a');
                document.body.appendChild(pom); //required in FF, optional for Chrome
                pom.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(JSON.stringify(config)));
                pom.download = filenameDisplay;
                pom.target="_self" ; //required in FF, optional for Chrome
                pom.click();
            }
        });
    });

    //Display on/off buttons
    $(function () {
        $(".displayToggleButtons").on("click", function (e) {
            var checkbox = $(this);

            var on = checkbox.parent().parent().find(".onButton");
            var off = checkbox.parent().parent().find(".offButton");
            var waitTime = checkbox.parent().parent().parent().find(".waitTime");
            var durationTime = checkbox.parent().parent().parent().find(".durationTime");
            var alarmLevel = checkbox.parent().parent().parent().find(".level");

            alarmLevel.toggle();
            on.toggle();
            off.toggle();
            waitTime.toggle();
            durationTime.toggle();
        });
    });

    $(function () {
        $(".eventButton").on("click", function (e) {
            var eventButton = $(this);

            if (self.lastEventClicked == null) {
                if (eventButton.is("#clockButton")) {
                    if (validateTimeInput() == true) {
                        eventButton.addClass('active');
                        self.lastEventClicked = eventButton;
                    }
                }
                else {
                    self.lastEventClicked = eventButton
                    eventButton.addClass('active');
                }
            }

            if (self.lastEventClicked != eventButton) {
                if (eventButton.is("#clockButton")) {
                    if (validateTimeInput() == true) {
                        self.lastEventClicked.removeClass('active');
                        self.lastEventClicked = eventButton;
                        eventButton.addClass('active');
                    }
                }
                else {
                    self.lastEventClicked.removeClass('active');
                    self.lastEventClicked = eventButton;
                    eventButton.addClass('active');
                }
            }
        });
    });

    $('#timeInput').keypress(function (e) {
        if (e.which == 13) {
            if (validateTimeInput() == false)
                alert("Clock event is not in format: hh:mm");
            e.preventDefault;
        } 
    });

    function validateTimeInput() {
        var time = $('#timeInput');
        var regex = /^[0-9][0-9][:][0-9][0-9]$/;

        if (regex.test(time.val())) {
            time.blur();
            return true;
        }
        else {
            return false;
        }
    }

    function tinypack (type, length, data) {
        this.type = type;
        this.length = length;
        this.data = data;
    }

    $(function () {
        $(".onButton").on("click", function (e) {
            var on = $(this);
            var off = on.parent().find(".offButton");

            on.addClass("onToggled");

            if (off.hasClass("offToggled") == true) {
                off.removeClass("offToggled");
            }
        });
    });

    $(function () {
        $(".offButton").on("click", function (e) {
            var off = $(this);
            var on = off.parent().find(".onButton");

            off.addClass("offToggled");

            if (on.hasClass("onToggled") == true) {
                on.removeClass("onToggled");
            }
        });
    });
