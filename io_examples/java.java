import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        // Your final solution should use System.in, but for testing you may
        // want to read from a file.
        // Scanner scanner = new Scanner(new File("/some/file"));
        Scanner scanner = new Scanner(System.in);

        int p = scanner.nextInt();
        int l = scanner.nextInt();
        for (int i = 0; i < l; i++) {
            int a = scanner.nextInt();
            int b = scanner.nextInt();
            // Save the connection (a, b).
        }

        // Run your algorithm.
        int solution = 4;

        // Print your solution.
        System.out.println(solution);
    }
}
