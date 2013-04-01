import java.util.ArrayList;
import java.util.HashMap;

public class DependenciesCollection {
	//public Set<(String,String)> dependencies;
	//description of layers: numberofsentence((wordID, [wordIDs]))
	public ArrayList<HashMap<Integer, ArrayList<Integer>>> leftChildren;
	public ArrayList<HashMap<Integer, ArrayList<Integer>>> rightChildren;
	public ArrayList<HashMap<Integer, Integer>> parents;
	public int numSentences;
	//public HashSet<String> children;
	
	public DependenciesCollection() {
		leftChildren = new ArrayList<HashMap<Integer, ArrayList<Integer>>>();
		rightChildren = new ArrayList<HashMap<Integer, ArrayList<Integer>>>();
		parents = new ArrayList<HashMap<Integer, Integer>>();
		
		leftChildren.add(new HashMap<Integer, ArrayList<Integer>>());
		rightChildren.add(new HashMap<Integer, ArrayList<Integer>>());
		parents.add(new HashMap<Integer, Integer>());
		numSentences = 0;
	}
	
	public HashMap<Integer, ArrayList<Integer>> getLeftChildren(int sentence) {
		return leftChildren.get(sentence);
	}
	
	public HashMap<Integer, ArrayList<Integer>> getRightChildren(int sentence) {
		return rightChildren.get(sentence);
	}
	
	public HashMap<Integer, Integer> getParent(int sentence) {
		return parents.get(sentence);
	}
	
	public void newSentence() {
		leftChildren.add(new HashMap<Integer, ArrayList<Integer>>());
		rightChildren.add(new HashMap<Integer, ArrayList<Integer>>());
		parents.add(new HashMap<Integer, Integer>());
		numSentences++;
	}
	
	public void addAll(ArrayList<ArrayList<String[]>> conllSentences) {
		for (ArrayList<String[]> conllSentence: conllSentences) {
			for (String[] word: conllSentence) {
				Integer parentID = Integer.parseInt(word[CoNLLFieldType.HEAD.ordinal()]);
				//skip the ROOT
				if (parentID >= 0) {
					Integer childID = Integer.parseInt(word[CoNLLFieldType.ID.ordinal()]);
					add(parentID, childID);
				}
			}
		}
	}
	
	public void add(Integer parentID, Integer childID) {
		//dependencies.add((parent[CoNLLFieldType.ID], child[CoNLLFieldType.ID]));
		//System.out.println(parentID);
		//System.out.println(childID);
		parents.get(numSentences).put(childID, parentID);
			
		if (parentID < childID) {
			if (!rightChildren.get(numSentences).containsKey(parentID)) {
				rightChildren.get(numSentences).put(parentID, new ArrayList<Integer>());
			}
			rightChildren.get(numSentences).get(parentID).add(childID);
		}
		else if (parentID > childID) {
			if (!leftChildren.get(numSentences).containsKey(parentID)) {
				leftChildren.get(numSentences).put(parentID, new ArrayList<Integer>());
			}
			leftChildren.get(numSentences).get(parentID).add(childID);
		}
		else {
			//Shouldn't be here
			//Means that parentID == childID, or we're comparing the same words in a sentence
		}
	}
	
	
}
