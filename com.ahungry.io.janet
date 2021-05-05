(import ./build/com_ahungry_io :as io)

(pp "Press the any key to continue...")
(pp (io/wait-for-key))
(pp "done")
