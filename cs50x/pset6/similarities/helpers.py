from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # TODO
    matrix = []
    
    # first cell
    matrix.append([(0, None)])

    # first row
    for j in range(1, len(b) + 1):
        matrix[0].append((j, Operation(2)))
    
    # first column
    for i in range(1, len(a) + 1):
        matrix.append([(i, Operation(1))])
    
    for j in range(1, len(b) + 1):
        for i in range(1, len(a) + 1):
            a_list = list(a[:i])
            a_list.insert(0, "")
            b_list = list(b[:j])
            b_list.insert(0, "")
            deletion = (matrix[i - 1][j][0] + 1, Operation(1))
            insertion = (matrix[i][j - 1][0] + 1, Operation(2))
            if a_list[i] == b_list[j]:
                substitution = (matrix[i - 1][j - 1][0], Operation(3))
            else:
                substitution = (matrix[i - 1][j - 1][0] + 1, Operation(3))
            
            if deletion[0] < insertion[0]:
                if deletion[0] < substitution[0]:
                    matrix[i].append(deletion)
                else:
                    matrix[i].append(substitution)
            else:
                if insertion[0] < substitution[0]:
                    matrix[i].append(insertion)
                else:
                    matrix[i].append(substitution)
    
    return matrix
