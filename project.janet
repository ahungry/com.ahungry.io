(declare-project
 :name "com.ahungry.io"
 :description "I/O Utilities for Janet"
 :author "Matthew Carter"
 :license "MIT"
 :dependencies []
 :url "https://github.com/ahungry/com.ahungry.io/"
 :repo "git+https://github.com/ahungry/com.ahungry.io.git")

(declare-source
 :name "com.ahungry.io"
 :source ["com.ahungry.io.janet"])

(declare-native
  :name "com_ahungry_io"
  :cflags ["-Wall" "-Wextra"]
  :lflags ["-pthread"]
  # :embedded @["pobox_lib.janet"]
  :source @["com_ahungry_io.c"])
