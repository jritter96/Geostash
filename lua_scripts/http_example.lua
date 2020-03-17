require("httpserver").createServer(80, function(req, res)
  co = coroutine.create(function(res, data)
    res:send(nil, 200)
    res:send_header("Access-Control-Allow-Origin", "*");
    res:send_header("Connection", "close")
    res:finish(data)
  end)
  req.ondata = function(self, chunk)
    print(chunk)
    if not chunk then
      str = nil
      uart.on("data", "\r",
        function(data)
          uart.on("data") -- unregister callback function
          str = data
          coroutine.resume(co, res, str)
      end, 0)
    end
  end
end)
