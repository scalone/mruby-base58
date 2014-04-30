##  
## Base58 Test
##

assert("Base58#encode") do
  t = Base58.encode "el psy congroo"
  assert_equal("DhB9KczCGdz4er3fWRe", t)
  t = Base58.encode "\0\1\2\3\4\5\6", Base58::BITCOIN_SYMBOLS
  assert_equal("1W7LcTy7", t)
end

assert("Base58#decode") do
  t = Base58.decode "DhB9KczCGdz4er3fWRe"
  assert_equal("el psy congroo", t)
  t = Base58.decode "1W7LcTy7", Base58::BITCOIN_SYMBOLS
  assert_equal("\0\1\2\3\4\5\6", t)
end
