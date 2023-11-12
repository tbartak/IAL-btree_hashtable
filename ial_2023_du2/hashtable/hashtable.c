/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
   if(table != NULL)
  {
    for(int i = 0; i < HT_SIZE; i++) // inicializujeme tabulku hodnotami NULL
    {
      (*table)[i] = NULL;
    }
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  if (!(table == NULL || key == NULL))
  {
    int index = get_hash(key); // ziskani indexu daneho hashovaci funkci
    ht_item_t* tmp = (*table)[index]; // ulozime si aktualni prvek do pomocne promenne
    while(tmp != NULL)
    {
      if(strcmp(key, tmp->key) == 0)
      {
        return tmp; // pokud byl prvek nalezen, vratime ho
      }
      tmp = tmp->next; // presun na nasledujici prvek
    }
  }
  return NULL; // prvek nebyl nalezen
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (!(table == NULL || key == NULL))
  {
    ht_item_t* currentItem = ht_search(table, key);
    if(currentItem != NULL) // pokud search funkce vrati NULL, znamena to, ze prvek se v tabulce nenachazi
    {
      currentItem->value = value; // pokud byl nalezen, nahradime jeho hodnotu 
    }
    else //pridavame prvek, ktery jeste v tabulce nebyl
    {
      ht_item_t* newItem = malloc(sizeof(ht_item_t));
      if (newItem == NULL)
      {
        return;
      }
      newItem->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
      if (newItem->key == NULL)
      {
        free(newItem);
        return;
      }

      int index = get_hash(key);
      // vkladani hodnot do noveho prvku
      strcpy(newItem->key, key); 
      newItem->value = value;
      newItem->next = (*table)[index];
      (*table)[index] = newItem; // vlozeni prvku do tabulky
    }
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (!(table == NULL || key == NULL))
  {
    ht_item_t* searchedItem = ht_search(table, key); // pokud se prvek v tabulce nachazi, tak se ulozi do searchedItem, jinak tam bude NULL
    if (searchedItem != NULL)
    {
      return &(searchedItem->value); // v pripade ze funkce ht_search polozku nasla, tak vracime ukazatel na hodnotu prvku
    }
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (!(table == NULL || key == NULL))
  {
    int index = get_hash(key); // ziskame index daneho prvku, ktery chceme smazat
    ht_item_t* currentItem = (*table)[index];
    ht_item_t* previousItem = NULL;
    
    while (currentItem != NULL) // pokud existuje prvek
    {
      if (strcmp(currentItem->key, key) == 0)
      {
        if (previousItem == NULL) // pokud je prvek prvni v seznamu/nema predchudce, tak nastavime zacatek seznamu na dalsi prvek v poradi
        {
          (*table)[index] = currentItem->next;
        }
        else // preskocime smazany prvek
        {
          previousItem->next = currentItem->next;
        }
        free(currentItem->key);
        free(currentItem);
        return;
      }
      // posun na dalsi prvky
      previousItem = currentItem;
      currentItem = currentItem->next;
    }
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  if (table != NULL)
  {
    for (int i = 0; i < HT_SIZE; i++) // prochazime vsechny prvky
    {
      ht_item_t* nextItem;
      while ((*table)[i] != NULL) // dokud prvek existuje
      {
        // posouvame se dal a mazeme naalokovanou pamet
        nextItem = (*table)[i]->next;
        free((*table)[i]->key);
        free((*table)[i]);
        (*table)[i] = nextItem;
      }
    }
  }
}
