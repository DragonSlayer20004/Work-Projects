
using System.Collections.Generic;

namespace Practice_Code_Methods
{
    internal class Program
    {

        static void Main(string[] args)
        {
            Console.WriteLine("Welcome to my personal practice");
            Console.WriteLine();
            ReverseLinkedList();
        }

        // Reverse list practice main method.
        static private void ReverseLinkedList()
        {
            // Create a monster list to test.
            LinkedList<Monsters> monsterList = new LinkedList<Monsters>();

            monsterList.AddLast(new Goblin("Grackle"));
            monsterList.AddLast(new Centaur("Taurus"));
            monsterList.AddLast(new Goblin("Gobo"));
            monsterList.AddLast(new Centaur("Hooby"));
            monsterList.AddLast(new Goblin("Geezer"));

            // Use overide PrintList to print the monster attacks.
            PrintList("Monsters", monsterList);

            // Create a string list to test.
            LinkedList<string> stringList = new LinkedList<string>();

            stringList.AddLast("Hello");
            stringList.AddLast("There");
            stringList.AddLast("How");
            stringList.AddLast("Are");
            stringList.AddLast("You");

            PrintList("Strings", stringList);

            // Reverse the lists.
            monsterList = reverseList(monsterList);
            stringList = reverseList(stringList);

            // Print out the reversed lists.
            PrintList("Monsters", monsterList);
            PrintList("Strings", stringList);
        }

        // Reverse the list.
        static private LinkedList<T> reverseList<T>(LinkedList<T> list) {
            // Create new list.
            LinkedList<T> newList = new LinkedList<T>();

            // Option 1 for reading in all the values: Grab the last value and put it in the new List then delete it from the original list.
            while (list.Count > 0)
            {
                //Check for Null first.
                if (list.Last != null)
                {
                    newList.AddLast(list.Last.Value);
                    list.RemoveLast();
                }
            }

            return newList;
        }

        // Print a linked list.
        private static void PrintList<T>(string title, LinkedList<T> list)
        {
            Console.WriteLine(title);

            // Option 2 for reading in all the values: Use a for each to grab each item in the list and iterate over them.
            foreach (var item in list)
            {
                Console.Write(item + " -> ");
            }
            Console.WriteLine("NULL");
            Console.WriteLine();
        }

        // Override and print a monster linked list by attacking the player.
        private static void PrintList(string title, LinkedList<Monsters> list)
        {
            Console.WriteLine(title);

            // Option 2 for reading in all the values: Use a for each to grab each item in the list and iterate over them.
            foreach (Monsters item in list)
            {
                item.AttackPlayer();
            }
        }
    }

    // Generic class of monsters with a few base parameters and methods.
    public abstract class Monsters {
        public string? Name { get; set; }
        public string? Attack { get; set; }
        public int DamageAmount { get; set; }

        // Prints a statement explaining how the monster attacks and for how much damage.
        public void AttackPlayer()
        {
            System.Console.WriteLine(Name + " Attacks with " + Attack + "!");
            System.Console.WriteLine(Name + " Deals " + DamageAmount + " damage!");
            System.Console.WriteLine();
        }
    }

    public class Goblin : Monsters {
        public Goblin(string _name)
        {
            Name = _name;
            Attack = "dagger";
            DamageAmount = 4;
        }
    }

    public class Centaur : Monsters
    {
        public Centaur(string _name)
        {
            Name = _name;
            Attack = "Spear";
            DamageAmount = 15;
        }
    }
}
