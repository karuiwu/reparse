import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;

public class OracleTest {
	
	public ArrayList<ArrayList<String[]>> conllSentences;
	//using and ArrayList as opposed to Queue/Stack implementation to simplify union and intersection methods
	public ArrayList<Integer> stack;
	public ArrayList<Integer> buffer;
	public DependenciesCollection dependencies;
	int currentSentenceNum;
	
	public OracleTest() {
		conllSentences = new ArrayList<ArrayList<String[]>>();
		dependencies = new DependenciesCollection();
	}
	
	/**
	 * Reads in the file containing CoNLL training data
	 * @param file
	 */
	public void readIn(String file) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String line;
			int sentenceNum = 0;
			conllSentences.add(new ArrayList<String[]>());
			conllSentences.get(sentenceNum).add(
					new String[]{"0", "ROOT", "-", "-", "-", "-", "-1", "-", "-", "-"});
			while ((line = reader.readLine()) != null) {
				line = line.trim();
				//testing for newline character "\n" or empty string "" doesn't work
				if (line.isEmpty())  {
					dependencies.addAll(conllSentences);
					dependencies.newSentence();
					/*for (String[] word: conllSentences.get(sentenceNum)) {
						System.out.print(Arrays.toString(word) + " ");
					}
					System.out.println();*/
					
					sentenceNum++;
					conllSentences.add(new ArrayList<String[]>());
					conllSentences.get(sentenceNum).add(
							new String[]{"0", "ROOT", "-", "-", "-", "-", "-1", "-", "-", "-"});
				}
				else {
					conllSentences.get(sentenceNum).add(line.split("\t"));
				}
			}
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	/**
	 * Finds the parser actions with the minimum cost
	 * @return
	 */
	public ArrayList<Integer> nextAction() {
		System.out.println("Cost of Left-Arc: " + costOfLeftArc());
		System.out.println("Cost of Right-Arc: " + costOfRightArc());
		System.out.println("Cost of Reduce: " + costOfReduce());
		System.out.println("Cost of Shift: " + costOfShift());
		
		ArrayList<Integer> costs = new ArrayList<Integer>();
		costs.add(costOfLeftArc());
		costs.add(costOfRightArc());
		costs.add(costOfReduce());
		costs.add(costOfShift());
		
		Integer minimumCost = Collections.min(costs);
		ArrayList<Integer> actions = new ArrayList<Integer>();
		for (int i = 0; i < costs.size(); i++) {
			if (costs.get(i) == minimumCost) {
				actions.add(i);
			}
		}
		
		return actions;
	}
	
	public int costOfLeftArc() {
		if (stack.isEmpty() || buffer.isEmpty()) {
			return Integer.MAX_VALUE;
		}
		else {
			Integer topOfStack = stack.get(stack.size()-1);
			int cost = 0;
			Integer parentID = dependencies.getParent(currentSentenceNum).get(topOfStack);
			if (parentID != buffer.get(0) && buffer.contains(parentID)) {
				cost++;
			}
			ArrayList<Integer> rightChildrenIDs = dependencies.getRightChildren(currentSentenceNum).get(topOfStack);
			if (rightChildrenIDs != null) {
				ArrayList<Integer> rightChildrenInBuffer = (ArrayList<Integer>) buffer.clone();
				rightChildrenInBuffer.retainAll(rightChildrenIDs);
				cost += rightChildrenInBuffer.size();
			}
			
			return cost;
		}
	}
	
	public int costOfRightArc() {
		if (stack.isEmpty() || buffer.isEmpty()) {
			return Integer.MAX_VALUE;
		}
		else {
			Integer topOfBuffer = buffer.get(0);
			int cost = 0;
			Integer parentID = dependencies.getParent(currentSentenceNum).get(topOfBuffer);
			if ((parentID != stack.get(stack.size()-1) && stack.contains(parentID)) || buffer.contains(parentID)) {
				cost++;
			}
			ArrayList<Integer> leftChildrenIDs = dependencies.getLeftChildren(currentSentenceNum).get(topOfBuffer);
			if (leftChildrenIDs != null) {
				ArrayList<Integer> leftChildrenInStack = (ArrayList<Integer>) stack.clone();
				leftChildrenInStack.retainAll(leftChildrenIDs);
				cost += leftChildrenInStack.size();
			}
			
			return cost;
		}
	}
	
	public int costOfReduce() {
		if (stack.size() < 2) {
			return Integer.MAX_VALUE;
		}
		else {
			Integer topOfStack = stack.get(0);
			int cost = 0;
			ArrayList<Integer> rightChildrenIDs = dependencies.getRightChildren(currentSentenceNum).get(topOfStack);
			if (rightChildrenIDs != null) {
				ArrayList<Integer> rightChildrenInBuffer = (ArrayList<Integer>) buffer.clone();
				rightChildrenInBuffer.retainAll(rightChildrenIDs);
				cost += rightChildrenInBuffer.size();
			}
			
			return cost;
		}
	}
	
	public int costOfShift() {
		if (buffer.isEmpty()) {
			return Integer.MAX_VALUE;
		}
		else {
			Integer topOfBuffer = buffer.get(0);
			int cost = 0;
			Integer parentID = dependencies.getParent(currentSentenceNum).get(topOfBuffer);
			if (stack.contains(parentID)) {
				cost++;
			}
			ArrayList<Integer> leftChildrenIDs = dependencies.getLeftChildren(currentSentenceNum).get(topOfBuffer);
			if (leftChildrenIDs != null) {
				ArrayList<Integer> leftChildrenInStack = (ArrayList<Integer>) stack.clone();
				leftChildrenInStack.retainAll(leftChildrenIDs);
				cost += leftChildrenInStack.size();
			}
			
			return cost;
		}
	}
	
	//region Lazy implementations of parser actions since we don't need to information about the arcs at every step ... yet
	public void leftArc() {
		stack.remove(stack.size()-1);
	}
	
	public void rightArc() {
		stack.add(buffer.get(0));
		buffer.remove(0);
	}
	
	public void reduce() {
		stack.remove(stack.size()-1);
	}

	public void shift() {
		stack.add(buffer.get(0));
		buffer.remove(0);
	}
	//endregion
	
	/**
	 * Lazy implementation of pickActions since we don't need to add more sophistication ... yet
	 * @param actions
	 * @return the action
	 */
	public Integer pickAction(ArrayList<Integer> actions) {
		return actions.get(0);
	}
	
	public void parseAll() {
		for (currentSentenceNum = 0; currentSentenceNum < conllSentences.size(); currentSentenceNum++) {
			stack = new ArrayList<Integer>();
			buffer = new ArrayList<Integer>();
			for (String[] word: conllSentences.get(currentSentenceNum)) {
				buffer.add(Integer.parseInt(word[CoNLLFieldType.ID.ordinal()]));
			}
			System.out.println(buffer);
			
			while (stack.size() > 1 || !buffer.isEmpty()) {
				ArrayList<Integer> possibleActions = nextAction();
				System.out.println(possibleActions);
				Integer action = pickAction(possibleActions);
				if (action == ParserActionType.LEFTARC.ordinal()) {
					leftArc();
				}
				else if (action == ParserActionType.RIGHTARC.ordinal()) {
					rightArc();
				}
				else if (action == ParserActionType.REDUCE.ordinal()) {
					reduce();
				}
				else if (action == ParserActionType.SHIFT.ordinal()) {
					shift();
				}
			}

			System.out.println("\n===========\n");
		}
	}
	
	/**
	 * @param args: filename
	 */
	public static void main(String[] args) {
		OracleTest oracleTest = new OracleTest();
		oracleTest.readIn(args[0]);
		oracleTest.parseAll();
		
		System.out.println(oracleTest.dependencies.parents);
	}
}
