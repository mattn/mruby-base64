
assert('Base64 encode "ruby"') do
  Base64::encode('ruby') == 'cnVieQ=='
end

assert('Base64 decode for old example') do
  Base64::decode('44GK5YmN44Gv44Gp44GT44Gu44Ov44Kr44Oh44GY44KD') == "お前はどこのワカメじゃ"
end

assert('Base64 decode null characters') do
  Base64::decode('AGZvbw==') == "\000foo"
end
