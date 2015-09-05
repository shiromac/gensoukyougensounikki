sstr = StyleString()

key = random()*2

if (key >= 1) then
	textSetPath(sstr, "Pattern1")
else
	textSetPath(sstr, "Pattern2")
end

textPrint(sstr)