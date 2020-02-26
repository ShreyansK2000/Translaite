-- This information is used by the Wi-Fi dongle to make a wireless connection to the router in the Lab
-- or if you are using another router e.g. at home, change ID and Password appropriately
SSID = "Shrey" -- WIFI NAME
SSID_PASSWORD = "antivenom123" -- WIFI PASSWORD

PI_IP = "192.168.43.31" -- PI IP ADDRESS
FLASK_PORT = "5000" -- FLASK PORT NUMBER
TRANSLATE_ENDPOINT = '/translate?'
TARGET_PARAMETER = 'target_language='
NATIVE_PARAMETER = '&native_language='

AUDIO_ENDPOINT = '/play_audio?'
WORD_PARAMETER = 'word='
LANGUAGE_PARAMETER = '&language='

REGISTER_ENDPOINT = '/register_user?'
USERNAME_PARAMETER = 'name='
PASSWORD_PARAMETER = '&password='

LOGIN_ENDPOINT = '/authenticate_user?'

ADD_HISTORY_ENDPOINT = '/add_to_history?'
NATIVE_LANGUAGE_PARAMETER = '&native_language='
TARGET_LANGUAGE_PARAMETER = '&target_language='
NATIVE_WORD_PARAMETER = '&native_word='
TARGET_WORD_PARAMETER = '&target_word='

REMOVE_HISTORY_ENDPOINT = '/remove_from_history?'

GET_HISTORY_ENDPOINT = '/get_user_history?'

OPEN_BMP_SOCK_ENDPOINT = '/open_bmp_sock'

GET_LOCATION_ENDPOINT = '/get_location?'
LATITUDE_PARAMETER = 'latitude='
LONGITUDE_PARAMETER = '&longitude='


function listap(t)
      for ssid,v in pairs(t) do
        authmode, rssi, bssid, channel = 
          string.match(v, "(%d),(-?%d+),(%x%x:%x%x:%x%x:%x%x:%x%x:%x%x),(%d+)")
        print(ssid,authmode,rssi,bssid,channel)
      end
end

function connect_wifi()
    -- configure ESP as a station
    wifi.setmode(wifi.STATION)
    wifi.sta.config(SSID,SSID_PASSWORD)
    tmr.delay(1000000)
    
    for i=1,3 do
        wifi.sta.connect()
        tmr.delay(1000000)
        status = wifi.sta.status()
        if (wifi.sta.getip() ~= nil) then
            de1_respond("OK_CONNECT")
            -- Prints the IP given to ESP8266
            -- print(status)
            -- print(wifi.sta.getip())
            do return end
        end
    end

    de1_respond("ERR_CONNECT")
    -- wifi.sta.getap(listap)
end

function translate(target_language, native_language)
    http.get("http://"..PI_IP..":"..FLASK_PORT..TRANSLATE_ENDPOINT..TARGET_PARAMETER..target_language..NATIVE_PARAMETER..native_language.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function play_audio(word, language)
    http.get("http://"..PI_IP..":"..FLASK_PORT..AUDIO_ENDPOINT..WORD_PARAMETER..word..LANGUAGE_PARAMETER..language.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function register_user(username, password)
    http.get("http://"..PI_IP..":"..FLASK_PORT..REGISTER_ENDPOINT..USERNAME_PARAMETER..username..PASSWORD_PARAMETER..password.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function user_login(username, password)
    params = ""..USERNAME_PARAMETER..username..PASSWORD_PARAMETER..password..""
    http.get("http://"..PI_IP..":"..FLASK_PORT..LOGIN_ENDPOINT..params.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function add_to_history(username, native_language, target_language, native_word, target_word)
    params = ""..USERNAME_PARAMETER..username..NATIVE_LANGUAGE_PARAMETER..native_language..TARGET_LANGUAGE_PARAMETER..target_language..NATIVE_WORD_PARAMETER..native_word..TARGET_WORD_PARAMETER..target_word..""
    http.get("http://"..PI_IP..":"..FLASK_PORT..ADD_HISTORY_ENDPOINT..params.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function remove_from_history(username, native_language, target_language, native_word, target_word)
    params = ""..USERNAME_PARAMETER..username..NATIVE_LANGUAGE_PARAMETER..native_language..TARGET_LANGUAGE_PARAMETER..target_language..NATIVE_WORD_PARAMETER..native_word..TARGET_WORD_PARAMETER..target_word..""
    http.get("http://"..PI_IP..":"..FLASK_PORT..REMOVE_HISTORY_ENDPOINT..params.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function get_user_history(username)
    params = ""..USERNAME_PARAMETER..username..""
    http.get("http://"..PI_IP..":"..FLASK_PORT..GET_HISTORY_ENDPOINT..params.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function open_bmp_sock()
    http.get("http://"..PI_IP..":"..FLASK_PORT..OPEN_BMP_SOCK_ENDPOINT.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function get_location(latitude, longitude)
    http.get("http://"..PI_IP..":"..FLASK_PORT..GET_LOCATION_ENDPOINT..LATITUDE_PARAMETER..latitude..LONGITUDE_PARAMETER..longitude.."", nil, function(code, data)
    if (code < 0) then
      de1_respond("HTTP request failed")
    else
      de1_respond(data)
    end
  end)
end

function de1_respond(message)
    print(message)
    -- TO INDICATE END OF MESSAGE TO THE DE1, WE PRINT 4 STARS
    print("****")
end

function toggle_gpio()
    for i=0,10,1 do
        gpio.write(3, gpio.LOW)
        tmr.delay(100000)
        gpio.write(3, gpio.HIGH)
        tmr.delay(100000)
    end
end
