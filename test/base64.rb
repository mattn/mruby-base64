
assert('Base64 encode "ruby"') do
  assert_equal 'cnVieQ==', Base64::encode('ruby')
end

assert('Base64 decode for old example') do
  assert_equal "お前はどこのワカメじゃ", Base64::decode('44GK5YmN44Gv44Gp44GT44Gu44Ov44Kr44Oh44GY44KD')
end

assert('Base64 decode null characters') do
  assert_equal "\000foo", Base64::decode('AGZvbw==')
end

assert('Base64 decode for data including newlines') do
  json = '{"key_field_first": "key_field_first", "key_field_second": "key_field_second", "attribute_one": "attribute_one", "attribute_two": "attribute_two", "path": "/path/of/example/handler", "method": "GET"}'
  # Base64.encode64(json) #=> data # on MRI
  data = "eyJrZXlfZmllbGRfZmlyc3QiOiAia2V5X2ZpZWxkX2ZpcnN0IiwgImtleV9m\n" +
    "aWVsZF9zZWNvbmQiOiAia2V5X2ZpZWxkX3NlY29uZCIsICJhdHRyaWJ1dGVf\n" +
    "b25lIjogImF0dHJpYnV0ZV9vbmUiLCAiYXR0cmlidXRlX3R3byI6ICJhdHRy\n" +
    "aWJ1dGVfdHdvIiwgInBhdGgiOiAiL3BhdGgvb2YvZXhhbXBsZS9oYW5kbGVy\n" +
    "IiwgIm1ldGhvZCI6ICJHRVQifQ==\n"
  assert_equal json, Base64::decode(data)
end
