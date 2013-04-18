import sys
import os
import random

def delete_files(folder):
    for the_file in os.listdir(folder):
        file_path = os.path.join(folder, the_file)
        try:
            if os.path.isfile(file_path):
                os.unlink(file_path)
        except Exception, e:
            print e

if __name__=="__main__":
    megam_filename = sys.argv[1]
    num_sets = int(sys.argv[2])
    """
    percentage_ratio = 0.1
    if len(sys.argv) > 3:
        percentage_ratio = float(sys.argv[3])
    """
    
    data_folder = "data/classifier/"
    dev_folder = data_folder + "dev/"
    train_folder = data_folder + "train/"
    if not os.path.exists(data_folder):
        print "Creating directories ..."
        os.makedirs(data_folder)
        os.makedirs(dev_folder)
        os.makedirs(train_folder)
    else:
        print "Deleting previous files ..."
        delete_files(train_folder)
        delete_files(dev_folder)

    megam_file = open(megam_filename)
    #the term "sentences" is to be used lightly here; did not yet change to a better fitting word since we may merge code later
    sentences = megam_file.readlines()
    used_sentences = []
    unverified_sets = []

    dev_set_size = int(len(sentences)/num_sets)
    train_set_size = len(sentences) - dev_set_size

    print "Creating data sets ",

    indices = [i for i in xrange(len(sentences))]
    train_file = open(train_folder+"0", 'w')
    dev_file = open(dev_folder+"0", 'w')

    dev_indices = random.sample(indices, dev_set_size)                                                                                                       
    #remove any elements in dev_sentences from the next possible batch of dev_sentences                                                                          
    diff_indices = list(set(indices)-set(dev_indices))
    train_sentences = [sentences[i] for i in diff_indices] + used_sentences
    dev_sentences = [sentences[i] for i in dev_indices]
    
    dev_file.write("".join(dev_sentences))
    train_file.write("".join(train_sentences))                                               

    """
    for i in xrange(num_sets):
        num = str(i)
        sys.stdout.write(".")
        sys.stdout.flush()
        train_file = open(train_folder+num, 'w')
        dev_file = open(dev_folder+num, 'w')
        print len(sentences), " ", dev_set_size
        dev_sentences = random.sample(sentences, dev_set_size)

        #remove any elements in dev_sentences from the next possible batch of dev_sentences
        diff_sentences = list(set(sentences)-set(dev_sentences))
        train_sentences = diff_sentences + used_sentences
        unverified_sets.append([train_sentences, dev_sentences])

        dev_file.write("\n".join(dev_sentences))
        train_file.write("\n".join(train_sentences))

        sentences = diff_sentences
        used_sentences += dev_sentences
    print ""


    print "Verifying ..."
    #verifying the proper ratio of training to test sentences
    for i in xrange(len(unverified_sets)):
        if (train_set_size != len(unverified_sets[i][0])):
            raise MyError("train set size is not consistent")
        if (dev_set_size != len(unverified_sets[i][1])):
            raise MyError("dev set size is not consistent")
        for j in xrange(i):
            #checks that dev sets do not contain the same sentences
            if len(set(unverified_sets[j][1])-set(unverified_sets[j][1])) > 0:
                raise MyError("dev sets contain the same sentences")
    """
