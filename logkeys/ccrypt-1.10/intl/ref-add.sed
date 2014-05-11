/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ ccrypt / ccrypt /
  tb
  s/ $/ ccrypt /
  :b
  s/^/# Packages using this file:/
}
