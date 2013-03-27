import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;


public class OracleTest {
	
	public ArrayList<String[]> conllSentences;
	
	public OracleTest() {
		conllSentences = new ArrayList<String[]>();
	}
	
	/**
	 * Reads in the file containing CoNLL training data
	 * @param file
	 */
	public void readIn(String file) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(file));
			String line;
			while ((line = reader.readLine()) != null) {
				conllSentences.add(line.split("\t"));
				System.out.println(line);
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
	 * @param args: filename
	 */
	public static void main(String[] args) {
		OracleTest oracleTest = new OracleTest();
		oracleTest.readIn(args[0]);
		

	}
}
