# pingu
ping ping ping ping

-> Generates continous ping requests to the target.
-> Does not care about the response.
-> High volume of ping requests slowly overloads the dut and it will slog down.
-> Usefull for testing if DUT is handling the simplest form of DOS attack.
-> Tested on my personal home router. It bogs down under ping requests in under 10 seconds and stop responding.
