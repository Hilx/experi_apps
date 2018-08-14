“<Begin_tran>key0:val0a\0key1:val1a\0…,keyN:valNa\0<Begin_tran>key0:va0b\0key4:val4b\0<End_tran>\0"
e.g. Bank Transaction A : John:+4000, Adam:-2300, Sarah:+100k
       Bank Transaction B : Ben:+35k, Adam:+4000
"<Begin_tran>John:4000\0Adam:-2300\0Sarah:100000\0<Begin_tran>Ben:35000\0Adam:4000\0<End_tran>\0"

Only the TransactionB should take effect and Transaction A should be aborted and (in the more realistic system the notification would be sent to the transaction source that it has to re-send it)

//Pseudo code
hash_t <string,integer> key_val_store;
// this implementation of the hash table does chaining, i.e., if there are two values for the same key, they would be stored as a linked list

list <string> key_list;//list to store keys to be rolled back if necessary

int main (char*data) {
  string key;
  int      value;
  while (*data) {
   if (Find <Begin_tran> == 1) {
     Skip_<Begin_tran>;
     while (key_value pairs exist) {
       extract_key_value(&key, &value, data);//updates data ptr
       add_to_hash_table(key, value);
       add_to_list(key);
       if (<End_tran> found) {
         //Commit
         move_last_values_in_the_key_value_store_linked_lists_to_first_pos_in_the_list(key_value_store,key_list);
         // Only the first values (values at the head) would be read and they represent the consistent state of the hash table
         remove_elements_from_the_list(key_list);
         break;
       } else if (<Begin_tran> found) {
         // Roll back
         remove_last_values_from_hash_table(key_value_store,key_list);// goes through the list and removes the  
                                                                                                              //corresponding elements from the hash table
         remove_elements_from_the_list(key_list);
         break;
       }
     }// while(key_value parts exist)
   }// if (Find)
 }// while (*data)
 
print_hash_table();

}// main()
