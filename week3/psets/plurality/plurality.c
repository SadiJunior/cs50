#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);
void merge_sort_candidates(void);
void merge_sort(int left_boundary, int right_boundary);
void merge(int left_boundary, int middle, int right_boundary);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: ./plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // If the given name matches the candidate name
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes += 1;

            // Returns true meaning the vote operation happened successfuly.
            return true;
        }
    }

    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Sorts the candidates array using the Merge Sort algorithm.
    merge_sort_candidates();

    // As the array is already sorted, the winners are already in the top (the first indexes).
    int highest_vote_count = candidates[0].votes;

    // Iteration for printing the name of the winners.
    for (int i = 0; i < candidate_count; i++)
    {
        // If the highest voted candidates have already passed (this can be done as the array is sorted).
        if (candidates[i].votes == highest_vote_count)
        {
            printf("%s\n", candidates[i].name);
        }
        else
        {
            return;
        }
    }
}

// Performs the Merge Sort algorithm by the candidate vote count (biggest to lowest).
void merge_sort_candidates(void)
{
    merge_sort(0, candidate_count - 1);
}

void merge_sort(int left_boundary, int right_boundary)
{
    // Stops the recursion when the left and right boundaries are the same.
    // This means that there is only one element there.
    if (left_boundary == right_boundary)
    {
        return;
    }

    int middle = (left_boundary + right_boundary) / 2;

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

    candidate left_portion[left_portion_size];
    candidate right_portion[right_portion_size];

    // Populates the left and right portion arrays.
    for (int i = 0; i < left_portion_size; i++)
    {
        left_portion[i] = candidates[i];
    }
    for (int i = 0; i < right_portion_size; i++)
    {
        right_portion[i] = candidates[i + middle + 1];
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
            candidates[i] = right_portion[top_right_index];
            top_right_index += 1;
        }
        else if (top_right_index >= right_portion_size)
        {
            candidates[i] = left_portion[top_left_index];
            top_left_index += 1;
        }
        // Compares the top left value with the top right value and passes the highest of them to the given array.
        else if (left_portion[top_left_index].votes > right_portion[top_right_index].votes)
        {
            candidates[i] = left_portion[top_left_index];
            top_left_index += 1;
        }
        else
        {
            candidates[i] = right_portion[top_right_index];
            top_right_index += 1;
        }
    }
}