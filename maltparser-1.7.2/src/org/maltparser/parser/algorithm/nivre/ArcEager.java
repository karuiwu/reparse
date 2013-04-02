package org.maltparser.parser.algorithm.nivre;

import java.util.Set;
import java.util.Stack;

import org.maltparser.core.exception.MaltChainedException;
import org.maltparser.core.symbol.SymbolTable;
import org.maltparser.core.syntaxgraph.edge.Edge;
import org.maltparser.core.syntaxgraph.node.DependencyNode;
import org.maltparser.parser.ParserConfiguration;
import org.maltparser.parser.TransitionSystem;
import org.maltparser.parser.history.GuideUserHistory;
import org.maltparser.parser.history.History;
import org.maltparser.parser.history.action.ComplexDecisionAction;
import org.maltparser.parser.history.action.GuideUserAction;
import org.maltparser.parser.transition.TransitionTable;

/**
 * @author Johan Hall
 * 
 */
public class ArcEager extends TransitionSystem {
	protected static final int SHIFT = 1;
	protected static final int REDUCE = 2;
	protected static final int RIGHTARC = 3;
	protected static final int LEFTARC = 4;

	public ArcEager() throws MaltChainedException {
		super();
	}

	public void apply(GuideUserAction currentAction, ParserConfiguration config)
			throws MaltChainedException {
		final NivreConfig nivreConfig = (NivreConfig) config;
		final Stack<DependencyNode> stack = nivreConfig.getStack();
		final Stack<DependencyNode> input = nivreConfig.getInput();
		currentAction.getAction(actionContainers);
		Edge e = null;

		// System.out.println("JUNEKI: stack " + stack);
		// System.out.println("JUNEKI: input " + input);

		System.out.print("JUNEKI STACK: \t\t");
		for (int i = 0; i < stack.size(); i++) {
			DependencyNode dNode = stack.get(i);
			//System.out.println(dNode);

			if (dNode.getLabelSet() != null) {
				Set<SymbolTable> tables = dNode.getLabelSet().keySet();

				for (SymbolTable table : tables) {
					try {
						if (table.getName().equals("FORM")) {
							System.out.print(dNode.getLabelSymbol(table) + " ");
						}
					} catch (MaltChainedException o) {

						o.getMessageChain();
					}
				}
			}
		}
		
		System.out.println();

		System.out.print("JUNEKI INPUT QUEUE: \t");
		for (int i = 0; i < input.size(); i++) {
			DependencyNode dNode = input.get(i);
			for (SymbolTable table : dNode.getLabelSet().keySet()) {
				try {
					if (table.getName().equals("FORM")) {
						// System.out.println(table.getName() + " "
						// + dNode.getLabelSymbol(table));
						System.out.print(dNode.getLabelSymbol(table) + " ");
					}
				} catch (MaltChainedException o) {
					o.getMessageChain();
				}
			}
		}
		System.out.println();

		switch (transActionContainer.getActionCode()) {
		case LEFTARC:
			e = nivreConfig.getDependencyStructure().addDependencyEdge(
					input.peek().getIndex(), stack.peek().getIndex());
			addEdgeLabels(e);
			stack.pop();
			// doPropagation(e);

			System.out.println(">> DECISION MADE: LEFTARC");

			break;
		case RIGHTARC:
			e = nivreConfig.getDependencyStructure().addDependencyEdge(
					stack.peek().getIndex(), input.peek().getIndex());
			addEdgeLabels(e);
			stack.push(input.pop());
			// doPropagation(e);

			System.out.println(">> DECISION MADE: RIGHTARC");

			break;
		case REDUCE:
			stack.pop();

			System.out.println(">> DECISION MADE: REDUCE");

			break;
		default:
			stack.push(input.pop());

			System.out.println(">> DECISION MADE: SHIFT OR OTHER");

			break;
		}
		
		
//		System.out.println("---------------------------------");
		
	}

	public GuideUserAction getDeterministicAction(GuideUserHistory history,
			ParserConfiguration config) throws MaltChainedException {
		final NivreConfig nivreConfig = (NivreConfig) config;

		// the .isRoot call does the right thing. Its an interface. And each
		// node correctly recalls if it is true or false

		if (!nivreConfig.isAllowRoot()
				&& nivreConfig.getStack().peek().isRoot()) {
			return updateActionContainers(history, ArcEager.SHIFT, null);
		}
		return null;
	}

	protected void addAvailableTransitionToTable(TransitionTable ttable)
			throws MaltChainedException {
		ttable.addTransition(SHIFT, "SH", false, null);
		ttable.addTransition(REDUCE, "RE", false, null);
		ttable.addTransition(RIGHTARC, "RA", true, null);
		ttable.addTransition(LEFTARC, "LA", true, null);
	}

	protected void initWithDefaultTransitions(GuideUserHistory history)
			throws MaltChainedException {
		GuideUserAction currentAction = new ComplexDecisionAction(
				(History) history);

		transActionContainer.setAction(SHIFT);
		transActionContainer.setAction(REDUCE);
		for (int i = 0; i < arcLabelActionContainers.length; i++) {
			arcLabelActionContainers[i].setAction(-1);
		}
		currentAction.addAction(actionContainers);
	}

	public String getName() {
		return "nivreeager";
	}

	public boolean permissible(GuideUserAction currentAction,
			ParserConfiguration config) throws MaltChainedException {
		currentAction.getAction(actionContainers);
		final int trans = transActionContainer.getActionCode();
		final DependencyNode stackPeek = ((NivreConfig) config).getStack()
				.peek();
		if ((trans == LEFTARC || trans == RIGHTARC)
				&& !isActionContainersLabeled()) {
			return false;
		}
		if ((trans == LEFTARC || trans == REDUCE) && stackPeek.isRoot()) {
			return false;
		}
		if (trans == LEFTARC && stackPeek.hasHead()) {
			return false;
		}
		if (trans == REDUCE && !stackPeek.hasHead()
				&& !((NivreConfig) config).isAllowReduce()) {
			return false;
		}
		return true;
	}

	public GuideUserAction defaultAction(GuideUserHistory history,
			ParserConfiguration configuration) throws MaltChainedException {
		return updateActionContainers(history, ArcEager.SHIFT, null);
	}
}