
assert('Base64 encode "ruby"') do
  assert_equal 'cnVieQ==', Base64::encode('ruby')
end

assert('Base64 decode for old example') do
  assert_equal "お前はどこのワカメじゃ", Base64::decode('44GK5YmN44Gv44Gp44GT44Gu44Ov44Kr44Oh44GY44KD')
end

assert('Base64 decode null characters') do
  assert_equal "\000foo", Base64::decode('AGZvbw==')
end
