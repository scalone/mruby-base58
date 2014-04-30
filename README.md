mruby-base58
============

mruby Base58 encoding

```ruby
Base58.encode "hello!"  
  # => "TZcKu5dH"
  
Base58.encode "hello!", Base58::BITCOIN_SYMBOLS  
  # => "tzCkV5Di"
  
Base58.encode "hello!", "ABCDEFGHJKLMNPQRSTUVWXYZ123456789abcdefghijkmnopqrstuvwxyz"
  # => "tzMk5ENi"
```

```ruby

Base58.decode "TZcKu5dH"
  # => "hello!"
  
Base58.decode "tzCkV5Di", Base58::BITCOIN_SYMBOLS  
  # => "hello!"
  
Base58.decode "tzMk5ENi", "ABCDEFGHJKLMNPQRSTUVWXYZ123456789abcdefghijkmnopqrstuvwxyz"
  # => "hello!"
```
