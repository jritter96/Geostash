-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately

--Mikayla's hotspot
SSID = "DESKTOP-5G7H0GH 0539"
SSID_PASSWORD = "8Jx97,34"

--Yuyi's hotspot
--SSID = "DESKTOP-T4VMAPH 6586"
--SSID_PASSWORD = "26Gm804("

-- I understand from students who have tried, that this also works, using UBC Visitor wireless connection
-- although I haven't tried it

-- SSID= "ubcvisitor"
-- SSID_PASSWORD = ""

-- configure ESP as a station
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,SSID_PASSWORD)
--wifi.sta.autoconnect(1)

-- alternatively you could do it this way
-- wifi.sta.config("M112-PD","aiv4aith2Zie4Aeg", 1)
wifi.sta.connect()

-- pause for connection to take place - adjust time delay if necessary or repeat until connection made
tmr.delay(1000000) -- wait 1,000,000 us = 1 second

-- This should print 5 if connection was successful
 print(wifi.sta.status())

-- List all available wireless network ---
-- See documentation: https://nodemcu.readthedocs.io/en/master/en/modules/wifi/#wifistagetap

-- The following 2 pieces of information are related to your Twilio account the one you made in Exercise 1.9
-- change appropriately
TWILIO_ACCOUNT_SID = "AC468dead258e80635ac0a5dcc5739b889"
TWILIO_TOKEN =       "895d5b6eb79511b8ebcfe557b1044d3d"

-- Unfortunately, the Wi-FI dongle can only make unsecured HTTP requests, but Twilio requires
-- secured HTTPS requests, so we will use a relay website to convert HTTP requests into HTTPS requests
-- visit http://iot-https-relay.appspot.com/ to learn more about this service
-- Please be sure to understand the security issues of using this relay app and use at your own risk.

-- this is the web address of the relay web site that our dongle sends the initial HTTP request to
HOST = "iot-https-relay.appspot.com"

-- The following variable defines the TWILIO web site that we will connect to
-- use the first one if you want to send a text to a cell phone
-- use the second (commented out) one if you want to make a call to a cell phone - that's the only change
URI = "/twilio/Messages.json"
--URI = "/twilio/Calls.json"

function build_post_request(host, uri, data_table)

     data = ""

     for param,value in pairs(data_table) do
          data = data .. param.."="..value.."&"
     end

     request = "POST "..uri.." HTTP/1.1\r\n"..
     "Host: "..host.."\r\n"..
     "Connection: close\r\n"..
     "Content-Type: application/x-www-form-urlencoded\r\n"..
     "Content-Length: "..string.len(data).."\r\n"..
     "\r\n"..
     data
     print(request)
     return request
end

-- This function registers a function to echo back any response from the server, to our DE1/NIOS system
-- or hyper-terminal (depending on what the dongle is connected to)
function display(sck,response)
     print(response)
end

-- When using send_sms: the "from" number HAS to be your twilio number.
-- If you have a free twilio account the "to" number HAS to be your twilio verified number.
function send_sms(from,to,body)

     data = {
      sid = TWILIO_ACCOUNT_SID,
      token = TWILIO_TOKEN,
      Body = string.gsub(body," ","+"),
      From = from,
      To = to
     }

     socket = net.createConnection(net.TCP,0)
     socket:on("receive",display)
     socket:connect(80,HOST)

     socket:on("connection",function(sck)
          post_request = build_post_request(HOST,URI,data)
          sck:send(post_request)
     end)
end

function check_wifi(lat, lon)
  ip = wifi.sta.getip()

 if(ip==nil) then
 else
  -- send a text message from, to, text
  send_sms("(347) 809-2765","(604) 362-0865","GeoStash is compromised! Last located at "..lat..", "..lon)

 end
end

function connect_wifi()
  wifi.sta.connect()

  -- pause for connection to take place - adjust time delay if necessary or repeat until connection made
  tmr.delay(1000000) -- wait 1,000,000 us = 1 second

  ip = wifi.sta.getip()

 if(ip==nil) then
  --nothing
 else
  tmr.stop(0)
  print(ip)

 end
end
