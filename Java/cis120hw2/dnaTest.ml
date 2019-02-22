;; open Dna
;; open Assert

(* The assertion library by default will run _all_ of the test cases associated
 * with this program.  However, while debugging you may prefer to have the
 * testing stop on the first failure that is encountered. Uncomment the line
 * below to turn on first-failure. *)  
(* ;; stop_on_failure () *)

(******************************************************************************)
(********************** REMEMBER TO WRITE TESTS FIRST! ************************)
(********************* WRITE ALL TESTS BELOW THIS POINT ***********************)
(******************************************************************************)

(************************ complementary_helix tests ***************************)

(* Here's an example test case.  Be sure to fill in the incomplete test cases
 * we provided. *)

let test () : bool =
  complementary_helix [T] = [A]
;; run_test "complementary_helix singleton" test

let test () : bool =
  complementary_helix [C; T; T; C] = [G; A; A; G]
;; run_test "complementary_helix multi-element list" test

let test () : bool =
  complementary_helix [C; T; A; A; T; G; T] = [G; A; T; T; A; C; A]
;; run_test "complementary_helix multi-element list with all nucleotides" test



(*************************** hamming_distance tests ***************************)

(* Here's an example test case.  Be sure to add your own--we will be grading the
 * tests for this problem. *)

let test () : bool =
  hamming_distance [G; A] [G; T] = 1
;; run_test "hamming_distance one different nucleotide" test

let test () : bool =
  hamming_distance [] [] = 0
;; run_test "hamming_distance empty lists" test

let test () : bool =
  hamming_distance [G; A; C; T; A; G; T] [G; T; C; T; C; C; C] = 4
;; run_test "hamming_distance multiple different nucleotides" test



(************************ decreasing_similarity tests *************************)

(* Here's an example test case for the list from question_most_like_human. Be
 * sure to add your own--we will be grading the tests for this problem. *)

let test () : bool =
  decreasing_similarity (question_most_like_human ())
;; run_test "question_most_like_human" test

let test () : bool =
  decreasing_similarity ([gorilla])
;; run_test "one entry decreasing_similarity" test

let test () : bool =
  decreasing_similarity ([])
;; run_test "empty list decreasing_similarity" test

let test () : bool =
   not (decreasing_similarity [pileated_gibbon; gorilla])
;; run_test "out of order - should fail - decreasing_similarity" test

let test () : bool =
  decreasing_similarity ([chimpanzee; gorilla; lar_gibbon])
;; run_test "multiple entry list decreasing_similarity" test



(**************************** acids_of_helix tests ****************************)

(* Here's an example test case.  Be sure to add your own--we will be grading the
 * tests for this problem.  Also be sure to fill in the incomplete test case
 * we provided. *)



let test () : bool =
  acids_of_helix [A; G; T; A; C; C; C; A; T; C; G; G] = [Met; Gly; Ser]
;; run_test "acids_of_helix three acids, no end codon" test

let test () : bool =
  acids_of_helix [A; G; T; A; C] = [Met]
;; run_test "acids_of_helix one acid, no end codon" test

let test () : bool =
  acids_of_helix [A; G; T; A; C; T; A; C; C; A; C; A; T; T] = [Met; Met; Val]
;; run_test "acids_of_helix multiple, end codon" test

let test () : bool =
  acids_of_helix [C; C; A; T; C; G; G] = []
;; run_test "acids_of_helix three acids, no start codon" test



(***************************** count_leaves tests *****************************)

(* Here are two example tests cases. Both trees should contain exactly four
 * species. Don't forget to add your own test cases! *)

let test () : bool =
  count_leaves (greater_apes ()) = 4
;; run_test "count_leaves lesser_apes" test

let test () : bool =
  count_leaves (lesser_apes ()) = 4
;; run_test "count_leaves lesser_apes" test

let test () : bool =
  count_leaves (
      Node (
        Leaf human,
        Leaf chimpanzee
      )
    ) = 2
;; run_test "count_leaves one node" test



(**************************** helix_of_tree tests *****************************)

(* Here's an example test case.  Be sure to add your own--we will be grading the
 * tests for this problem. *)

let test () : bool =
  helix_of_tree (LNode (LLeaf [T], [A], LLeaf [G])) = [A]
;; run_test "helix_of_tree lnode" test

let test () : bool =
  helix_of_tree (LNode (LNode(LLeaf [C], [A], LLeaf [G]), [C], LNode (LLeaf [T], [C], LLeaf [G]))) = [C]
;; run_test "helix_of_tree 3 depth tree " test

let test () : bool =
  helix_of_tree (LLeaf[G]) = [G]
;; run_test "helix_of_tree lleaf" test



(**************************** unlabel_tree tests ******************************)

(* Here's an example test case.  Be sure to add your own--we will be grading the
 * tests for this problem. *)

let test () : bool =
  unlabel_tree (LNode (LLeaf [T], [A], LLeaf [G])) =
    Node (Leaf [T], Leaf [G])
;; run_test "unlabel_tree depth-2 tree" test



(************************* guess_parent_helix tests ***************************)

(* Here's an example test case.  Be sure to fill in the incomplete test case
 * we provided and don't forget to add your own test cases! *)

let test () : bool =
  guess_parent_helix [T; C; A] [G; C; A] = [A; C; A]
;; run_test "guess_parent_helix one difference" test

let test () : bool =
  guess_parent_helix [A; C; G; G; T; A; C]
                     [C; T; G; C; T; A; A] = [A; A; G; A; T; A; A]
;; run_test "guess_parent_helix multiple differences" test

let test () : bool =
  guess_parent_helix [A; C; G; G]
                     [C; T; A; C] = [A; A; A; A]
;; run_test "guess_parent_helix all differences" test



(************************ add_ancestor_labels tests ***************************)

(* Here's an example test case.  Be sure to fill in the incomplete test cases
 * we provided and don't forget to add your own test cases! *)

let test () : bool =
  add_ancestor_labels (Node (Leaf [T], Leaf [G])) =
    LNode (LLeaf [T], [A], LLeaf [G])
;; run_test "add_ancestor_labels depth-2 tree" test

let test () : bool =
  add_ancestor_labels (Node (Leaf [T; C], Leaf [T; C])) =
    LNode (LLeaf [T; C], [T; C], LLeaf [T; C])
;; run_test "add_ancestor_labels also depth-2 tree" test

let test () : bool =
  add_ancestor_labels (Leaf[C]) =
    (LLeaf[C])
;; run_test "add_ancestor_labels leaf" test

let test () : bool =
  add_ancestor_labels (Node (Node(Leaf[C],Leaf[C]),Node(Leaf[C],Leaf[T]))) =
    (LNode (LNode(LLeaf[C],[C], LLeaf[C]),[A],LNode(LLeaf[C],[A],LLeaf[T])))
;; run_test "add_ancestor_labels depth-3 tree" test


(************************ parent_child_hamming tests **************************)

(* Here we give you one incomplete test to complete.  Add your own tests--we
 * will grade the test cases for this problem.  Be sure to test for trees of
 * greater depth. *)


let test () : bool =
  parent_child_hamming (LLeaf[T]) =
    0
;; run_test "parent_child_hamming leaf" test

let test () : bool =
  parent_child_hamming (LNode (LLeaf [T], [A], LLeaf [G])) =
    2
;; run_test "parent_child_hamming depth-2 tree, all different" test

let test () : bool =
  parent_child_hamming (LNode (LLeaf [T], [T], LLeaf [T])) =
    0
;; run_test "parent_child_hamming depth-2 tree, all the same" test

let test () : bool =
  parent_child_hamming (LNode (LNode(LLeaf[C],[C], LLeaf[C]),[A],LNode(LLeaf[C],[A],LLeaf[T]))) 
  = 3
;; run_test "parent_child_hamming depth-3 tree, some different" test

let test () : bool =
  parent_child_hamming (LNode (LLeaf [A], [T], LLeaf [C])) =
    2
;; run_test "parent_child_hamming depth-2 tree from simplest_tree" test



(**************************** simplest_tree tests *****************************)

(* Here are two example test cases. Don't forget to add your own test cases! *)

let test () : bool =
  simplest_tree [] = (LLeaf [], 0)
;; run_failing_test "simplest_tree empty" test

let test () : bool =
  let t1 = LNode (LLeaf [A], [T], LLeaf [C]) in
  let t2 = LNode (LLeaf [G; T], [A; T],
                  LNode (LLeaf [T; T], [T; T], LLeaf [C; G])) in
  simplest_tree [t1; t2] = (t1, 2)
;; run_test "simplest_tree two tree list" test

let test () : bool =
  let t1 = LNode (LLeaf [A], [T], LLeaf [C]) in
  let t2 = LNode (LLeaf [G; T], [A; T],
                  LNode (LLeaf [T; T], [T; T], LLeaf [C; G])) in
  let t3 = LNode (LLeaf [T; T], [T; T],
                  LNode (LLeaf [T; T], [T; T], LLeaf [T; T])) in
  simplest_tree [t1; t2; t3] = (t3, 0)
;; run_test "simplest_tree three tree list" test

let test () : bool =
  let t1 = LNode (LLeaf [T], [T], LLeaf [T]) in
  let t2 = LNode (LLeaf [G; T], [A; T],
                  LNode (LLeaf [T; T], [T; T], LLeaf [C; G])) in
  let t3 = LNode (LLeaf [T; G], [T; A], LLeaf [T; T]) in
  simplest_tree [t1; t2; t3] = (t1, 0)
;; run_test "simplest_tree second three tree list" test


(********************* simplest_greatest_ape_tree tests ***********************)

(* Refactoring is the process of rewriting your code to make it simpler and
 * more general without changing its behavior. Therefore, it is easy to write
 * test cases for the new version---they should compute the same answers
 * that we did before. *)

let test () : bool =
  find_simplest_tree gorilla human chimpanzee orangutan =
    simplest_greater_ape_tree ()
;; run_test "simplest_greater_ape_tree" test

let test () : bool =
  find_simplest_tree lar_gibbon pileated_gibbon siamang white_cheeked_gibbon =
    simplest_lesser_ape_tree ()
;; run_test "simplest_lesser_ape_tree" test

;; print_endline "dnaTest.ml: ran to completion."
