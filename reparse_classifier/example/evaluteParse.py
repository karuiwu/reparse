import sys

output_file = open(sys.argv[1])
gold_file = open(sys.argv[2])
#signifies which tab-separated column to use
column_id = 0

output_contents = output_file.readlines()
gold_contents = gold_file.readlines()

if len(output_contents) != len(gold_contents):
    print len(output_contents), len(gold_contents)
    raise Exception("Files are not the same length")

#true positive: correctly classified as 1
#true negative: correctly classified as 0
#false positive: incorrectly classified as 1
#false negative: incorrectly classified as 0
true_positive = true_negative = false_positive = false_negative = 0.0
for o, g in zip(output_contents, gold_contents):
    o_class = o[column_id]
    g_class = g[column_id]
    if g_class == "1":
        if o_class == "1":
            true_positive += 1
        elif o_class == "0":
            false_negative += 1
        else:
            raise Exception("Unrecognized class present: " + str(g_class))
        
    elif g_class == "0":
        if o_class == "1":
            false_positive += 1
        elif o_class == "0":
            true_negative += 1
        else:
            raise Exception("Unrecognized class present: " + str(g_class))
    else:
        raise Exception("Unrecognized class present: " + str(g_class))
        
print true_positive, true_negative, false_positive, false_negative

precision = true_positive/(true_positive + false_positive)
recall = true_positive/(true_positive + false_negative)

print precision, recall
