package org.maltparser.parser;

import org.maltparser.core.exception.MaltChainedException;
import org.maltparser.core.syntaxgraph.DependencyStructure;
import org.maltparser.parser.guide.ClassifierGuide;
import org.maltparser.parser.guide.SingleGuide;
import org.maltparser.parser.history.GuideHistory;
import org.maltparser.parser.history.action.GuideDecision;
import org.maltparser.parser.history.action.GuideUserAction;
import org.maltparser.parser.history.container.ActionContainer;

/**
 * @author Johan Hall
 * 
 */
public class DeterministicParser extends Parser {
	private int parseCount;

	public DeterministicParser(DependencyParserConfig manager)
			throws MaltChainedException {
		super(manager);
		setManager(manager);
		initParserState(1);
		((SingleMalt) manager).addRegistry(
				org.maltparser.parser.Algorithm.class, this);
		setGuide(new SingleGuide(manager,
				(GuideHistory) parserState.getHistory(),
				ClassifierGuide.GuideMode.CLASSIFY));
	}

	public DependencyStructure parse(DependencyStructure parseDependencyGraph)
			throws MaltChainedException {
		
		
		
		System.out.println();
		System.out.println("JUNEKI: parseDependencyGraph has a copy of our connll data: DeterministicParser.java line 37");
		System.out.println(parseDependencyGraph);
		System.out.println();

		
		
		
		
		if (diagnostics == true) {
			return parseDiagnostic(parseDependencyGraph);
		}
		parserState.clear();
		parserState.initialize(parseDependencyGraph);
		currentParserConfiguration = parserState.getConfiguration();
		parseCount++;
		TransitionSystem ts = parserState.getTransitionSystem();
		while (!parserState.isTerminalState()) {

			// If we have the root, we just shift it over. That is it.
			GuideUserAction action = ts.getDeterministicAction(
					parserState.getHistory(), currentParserConfiguration);

			// System.out.println("JUNEKI: parserState.getHistory() "+parserState.getHistory().getActionContainers());

			// We always enter this if statement. Because action always returns
			// null.
			if (action == null) {
				action = predict();
			}

			action.getAction(parserState.transitionSystem.actionContainers);
			int ACTIONCODE = parserState.transitionSystem.transActionContainer
					.getActionCode();

			
			parserState.apply(action);

//			System.out.println("JUNEKI: PREDICTED ACTION: " + action);
//			System.out.println("ACTIONCODE: " + ACTIONCODE);

			final int SHIFT = 1;
			final int REDUCE = 2;
			final int RIGHTARC = 3;
			final int LEFTARC = 4;

			System.out.print("PARSING DECISION MADE: ");
			switch (ACTIONCODE) {
			case LEFTARC:
				System.out.println("LEFTARC");
				break;
			case RIGHTARC:
				System.out.println("RIGHTARC");
				break;
			case REDUCE:
				System.out.println("REDUCE");
				break;
			case SHIFT:
				System.out.println("SHIFT");
				break;
			default:
				System.out.println("ELSE");

			}
			
			
			
			System.out.println("---------------------------------");

		}
		copyEdges(currentParserConfiguration.getDependencyGraph(),
				parseDependencyGraph);
		copyDynamicInput(currentParserConfiguration.getDependencyGraph(),
				parseDependencyGraph);
		parseDependencyGraph.linkAllTreesToRoot();
		return parseDependencyGraph;
	}

	private DependencyStructure parseDiagnostic(
			DependencyStructure parseDependencyGraph)
			throws MaltChainedException {
		parserState.clear();
		parserState.initialize(parseDependencyGraph);
		currentParserConfiguration = parserState.getConfiguration();
		parseCount++;
		if (diagnostics == true) {
			writeToDiaFile(parseCount + "");
		}
		while (!parserState.isTerminalState()) {
			GuideUserAction action = parserState.getTransitionSystem()
					.getDeterministicAction(parserState.getHistory(),
							currentParserConfiguration);
			if (action == null) {
				action = predict();
			} else if (diagnostics == true) {
				writeToDiaFile(" *");
			}
			if (diagnostics == true) {
				writeToDiaFile(" "
						+ parserState.getTransitionSystem().getActionString(
								action));
			}
			parserState.apply(action);
		}
		copyEdges(currentParserConfiguration.getDependencyGraph(),
				parseDependencyGraph);
		copyDynamicInput(currentParserConfiguration.getDependencyGraph(),
				parseDependencyGraph);
		parseDependencyGraph.linkAllTreesToRoot();
		if (diagnostics == true) {
			writeToDiaFile("\n");
		}
		return parseDependencyGraph;
	}

	private GuideUserAction predict() throws MaltChainedException {
		GuideUserAction currentAction = parserState.getHistory()
				.getEmptyGuideUserAction();
		try {
			classifierGuide.predict((GuideDecision) currentAction);
			while (!parserState.permissible(currentAction)) {
				if (classifierGuide
						.predictFromKBestList((GuideDecision) currentAction) == false) {
					currentAction = getParserState().getTransitionSystem()
							.defaultAction(parserState.getHistory(),
									currentParserConfiguration);
					break;
				}
			}
		} catch (NullPointerException e) {
			throw new MaltChainedException("The guide cannot be found. ", e);
		}
		return currentAction;
	}

	public void terminate() throws MaltChainedException {
		if (diagnostics == true) {
			closeDiaWriter();
		}
	}
}
