(import com.ahungry.io :as io)

(defn main [&]
  (pp "Press the any key to continue...")
  (def key1 (io/wait-for-key))
  (pp "Press any OTHER key to continue...")
  (while (= key1 (io/wait-for-key)) nil)
  (pp "done")
  (pp (string/format "You pushed the %c key at first" key1)))
