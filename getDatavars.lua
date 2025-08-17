local beaconState = 0

dataref("beacon", "sim/cockpit/electrical/beacon_lights_on", "readonly")

dataref("IAS", "sim/cockpit2/gauges/indicators/airspeed_kts_pilot", "readonly")
dataref("ALT", "sim/cockpit2/gauges/indicators/altitude_ft_pilot", "readonly")

dataref("HDG", "sim/flightmodel/position/mag_psi", "readonly") -- TODO: Get HDG from instrument



-- Require socket library
local socket = require("socket")

-- Arduino PC IP is localhost, pick any free port (e.g., 49000)
local UDP_IP = "127.0.0.1"
local UDP_PORT = 49000

udp = socket.udp()
udp:settimeout(0)
udp:setpeername(UDP_IP, UDP_PORT)

-- Send beacon state every frame
function send_IAS_udp()
    local msg = "IAS=" .. tostring(IAS)
    udp:send(msg)
end

function send_ALT_udp()
    local msg = "ALT=" .. tostring(ALT)
    udp:send(msg)
end

function send_HDG_udp()
    local msg = "HDG=" .. tostring(HDG)
    udp:send(msg)
end

function send_beacon_udp()
    local msg = "BEACON=" .. tostring(beacon)
    udp:send(msg)
end

do_every_frame("send_IAS_udp()")
do_every_frame("send_ALT_udp()")
do_every_frame("send_HDG_udp()")
--do_every_frame("send_beacon_udp()")