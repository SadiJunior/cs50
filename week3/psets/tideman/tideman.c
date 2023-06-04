#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void merge_sort_pairs(void);
void merge_sort(int left_boundary, int right_boundary);
void merge(int left_boundary, int middle, int right_boundary);
bool has_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Gets the index of the candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    // If the candidate was not found
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterates through the ranks
    for (int i = 0; i < candidate_count; i++)
    {
        // Gets the index of the given candidate (here he is preferred over the others)
        int preferred_index = ranks[i];

        // Iterates through all candidates after the given candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Gets the index of these candidates
            int not_preferred_index = ranks[j];

            // Updates the preferences counters
            preferences[preferred_index][not_preferred_index] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Iterates through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Gets the candidate to be compared with the first candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Gets the preferences count
            int first_candidate_count = preferences[i][j];
            int second_candidate_count = preferences[j][i];

            if (first_candidate_count == second_candidate_count)
            {
                continue;
            }

            if (first_candidate_count > second_candidate_count)
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
            }
            else
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
            }

            pair_count += 1;
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort_pairs();
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // If there is a cycle between winner and loser
        // Does this by verifying if all losers of the source loser (recursively) don't end up winning the source winner
        // (Yeah, this is really confusing)
        if (has_cycle(winner, loser))
        {
            continue;
        }

        locked[winner][loser] = true;
    }
}

// Print the winner of the election
void print_winner(void)
{
    // Finds and prints the winner
    for (int i = 0; i < candidate_count; i++)
    {
        bool has_locked = false;

        // Verifies if anyone wins over the given candidate
        for (int j = 0; j < candidate_count; j++)
        {
            // If someones wins him, then it is not the winner
            if (locked[j][i])
            {
                has_locked = true;
            }
        }

        // If no one wins him, you've found the winner
        if (!has_locked)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}

// Sorts the pairs array using the merge sort algorithm (decreasing order)
void merge_sort_pairs(void)
{
    merge_sort(0, pair_count - 1);
}

void merge_sort(int left_boundary, int right_boundary)
{
    // Stops the recursion when the left and right boundaries are the same.
    // This means that there is only one element there.
    if (left_boundary == right_boundary)
    {
        return;
    }

    int middle = left_boundary + right_boundary / 2;

    // Calls recusively merge_sort from the left and right portion of the given array.
    merge_sort(left_boundary, middle);
    merge_sort(middle + 1, right_boundary);

    // Merges and sorts both portions.
    merge(left_boundary, middle, right_boundary);
}

void merge(int left_boundary, int middle, int right_boundary)
{
    int left_portion_size = middle - left_boundary + 1;
    int right_portion_size = right_boundary - middle;

    pair left_portion[left_portion_size];
    pair right_portion[right_portion_size];

    // Populates the left and right portion arrays.
    for (int i = 0; i < left_portion_size; i++)
    {
        left_portion[i] = pairs[i];
    }
    for (int i = 0; i < right_portion_size; i++)
    {
        right_portion[i] = pairs[i + middle + 1];
    }

    // Stores the index of the top elements of both left and right portions.
    int top_left_index = 0;
    int top_right_index = 0;

    // Iterates through the indexes from left_boundary to right_boundary of the given array.
    for (int i = left_boundary; i <= right_boundary; i++)
    {
        // If has already passed all elements from either left or right portions.
        if (top_left_index >= left_portion_size)
        {
            pairs[i] = right_portion[top_right_index];
            top_right_index += 1;
        }
        else if (top_right_index >= right_portion_size)
        {
            pairs[i] = left_portion[top_left_index];
            top_left_index += 1;
        }
        else
        {
            // Gets the differences between the winner and the losers
            int left_winner = left_portion[top_left_index].winner;
            int left_loser = left_portion[top_left_index].loser;
            int left_item_difference = preferences[left_winner][left_loser] - preferences[left_loser][left_winner];

            int right_winner = right_portion[top_right_index].winner;
            int right_loser = right_portion[top_right_index].loser;
            int right_item_difference = preferences[right_winner][right_loser] - preferences[right_loser][right_winner];

            // Compares the top left value with the top right value and passes the highest of them to the given array.
            if (left_item_difference > right_item_difference)
            {
                pairs[i] = left_portion[top_left_index];
                top_left_index += 1;
            }
            else
            {
                pairs[i] = right_portion[top_right_index];
                top_right_index += 1;
            }
        }
    }
}

// Verifies if the locked array has a cycle
// This one is was really hard...
bool has_cycle(int winner, int loser)
{
    // If the loser is locked to the winner, it means one is locked to another, which means there is a cycle
    if (locked[loser][winner])
    {
        return true;
    }

    // Now I need to check if all the ones that are losers of the loser don't end up pointing to the winner
    // This will be done recursively
    for (int i = 0; i < candidate_count; i++)
    {
        // Gets the loser of the loser (which is also a loser of the source winner)
        if (locked[loser][i])
        {
            // Verifies if there is a cycle between the winner and the loser of the loser recursively
            if (has_cycle(winner, i))
            {
                return true;
            }
        }
    }

    return false;
}
